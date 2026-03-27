#include "../levels/Level1.h"
#include "../levels/Level2.h"
#include "../levels/Level3.h"
#include "../assets/SpritesBitmap.h"
#include "../engine/Sprite.h"
#include "../levels/LevelGifts.h"  // für drawGift usw.
#include "../assets/TmanBitmaps.h"

// Zugriff auf globale Variablen aus main.ino
extern Sprite trachtman;
extern void drawTile(int tileX, int tileY, uint8_t tileIndex);
extern void readInput();
extern void drawSpriteTransparent(Sprite &s, const uint16_t *bitmap, uint16_t transparentColor);
extern void saveSpriteBackground(Sprite &s);
extern bool movingLeft, movingRight;
extern bool play;
extern bool jumping;
extern Level* currentLevel;
extern const uint8_t keys;

int stop;

// ===== Level 2 Tiles + Gifts =====
Level level2 = {
    // Tiles
    {
        {15,16,17,17,17,17,17,15},
        {18,19,20,14,18,19,18,20},
        { 7, 7, 7, 7, 7, 7, 7,7},
        { 8, 5, 5, 5, 6, 8, 0,8},
        { 7, 7, 7, 7, 7, 7, 0,12},
        {11, 0,11,0, 11,11,0,11}
    },
    // Gifts
    {
        {DOGPOOP1, 50, 85, false, 0, 4000},
        {DOGPOOP2,120, 85, false, 0, 5000},
        {DOG,     180,85, false, 0, 3000},
        {QUESTION,200,45,false, 0, 5000},
        {WURST,    80,50,false, 0, 4000},
        {BRETZEL,  160,50,false, 0, 4000},
        {BIER,     100,70,false, 0, 6000},
        {GOLDKISTE,140,70,false,0, 8000}
    }
};

// ===== Level 2 Spezial-Logik =====
void updateLevel2() { 
    // im Keller links und rechts umkehren
    if(trachtman.x <= 0 && trachtman.y > 85) {movingLeft=false; movingRight=true;}
    if(trachtman.x >= 240 && trachtman.y > 85) {movingLeft=true; movingRight=false; drawTile(6,4,3);}
    // obere Ebene links umkehren
    if(trachtman.x <= 16 && trachtman.y <=85) {movingLeft=false; movingRight=true;}
    if(trachtman.x <= 16 && trachtman.y == 170) {
        trachtman.y = 85;
        drawTile(4,3,6);               // Brücke restore
        drawTile(6,2,7);
    }

    // Trachtman auf Anfangsposition
    if (trachtman.y == 117 || trachtman.y == 109) {
        drawTile(3,3,5);
        trachtman.y = 85;
        drawSpriteTransparent(trachtman, t3left, 0xFFFF);
    }

    // Brücke stürzt ein
    if (trachtman.x > 130 && trachtman.y == 85 && movingRight) {
        randomSeed(analogRead(29) * (3.3 / 4095) * 3);      // offenen ADC für echtes random
        int einsturz = random (0,3);
        if (einsturz > 1){ 
            drawTile(4,3,7);
            if (trachtman.y == 85) {
                movingRight = false;
                movingLeft = true;
                trachtman.y = 170;               // fällt in den Keller zum Hund
                drawTile(6,4,3);                 // Tonne unten geht auf
            }                 
        }
    }
    // Brezn und Bier erscheinen oben
    if (trachtman.x > 200 && trachtman.y == 85) {    
        LevelGift &bretzel = currentLevel->levelGifts[5];
        if (!bretzel.active) {             // nur einmal aktivieren
            bretzel.active = true;
            bretzel.spawnTime = millis();
            randomSeed(analogRead(29) * (3.3 / 4095) * 3);     // offenen ADC für echtes random
            int bretzel_x = 40 + 40*random (0,3);
            bretzel.x = bretzel_x;
            bretzel.y = 53;                // feste Y-Position
        }
        LevelGift &bier = currentLevel->levelGifts[6];
        if (!bier.active) {                // nur einmal aktivieren
            bier.active = true;
            bier.spawnTime = millis();
            bier.x = 200;                  // feste X-Position
            bier.y = 53;                   // feste Y-Position
        }
    }
    // Wurstsemmel, Bier  und Goldkiste erscheinen
    if (trachtman.x <= 0 && trachtman.y == 85) {    
        LevelGift &wurst = currentLevel->levelGifts[4];
        if (!wurst.active) {             // nur einmal aktivieren
            wurst.active = true;
            wurst.spawnTime = millis();
            wurst.x = 120;               // feste X-Position
            wurst.y = 170;               // feste Y-Position
        }
        LevelGift &bier = currentLevel->levelGifts[6];
        if (!bier.active) {             // nur einmal aktivieren
            bier.active = true;
            bier.spawnTime = millis();
            bier.x = 200;               // feste X-Position
            bier.y = 170;               // feste Y-Position
        }
        LevelGift &goldkiste = currentLevel->levelGifts[7];
        if (!goldkiste.active) {             // nur einmal aktivieren
            goldkiste.active = true;
            goldkiste.spawnTime = millis();
            goldkiste.x = 5;               // feste X-Position
            goldkiste.y = 170;               // feste Y-Position
        }
    }
    // durch den Schacht nach oben
    if(trachtman.x <= 85 && trachtman.y == 170 && movingRight) {
        LevelGift &dog = currentLevel->levelGifts[2];
        if (dog.active) {
            drawTile(6,4,3);               // Tonne unten geht auf
            drawTile(6,2,3);               // Tonne oben erscheint
            trachtman.x = 120;
            drawSpriteTransparent(trachtman, t3right, 0xFFFF);
            delay(250);
            drawTile(3,4,7);               // trachtman verschwindet
            delay(250);
            trachtman.y = 85;
            trachtman.x = 240;             // trachtman kommt rechts hoch
            drawTile(4,3,6);               // Brücke restore
            drawTile(6,2,7);
            //drawTile(6,4,0);             // Tonne unten geht zu
        }
    }
    // Schacht geht rechts wieder zu
    if (trachtman.x <= 0) {
        drawTile(6,2,7);
    }
    // Dog erscheint in linker Tonne
    if(trachtman.x >= 140 && trachtman.y == 170) {
        LevelGift &dog = currentLevel->levelGifts[2];
        if (!dog.active) {             // nur einmal aktivieren
            dog.active = true;
            dog.spawnTime = millis();
            dog.x = 40;               // Dog auf Tonne
            dog.y = 170;
        }
    }
    // ================================
    // == abgelaufene Gifts löschen ===
    // ================================
    // Dog löschen
    LevelGift &dog = currentLevel->levelGifts[2];
    if (millis() - dog.spawnTime > dog.lifetime && dog.active) {
        dog.active = false;
        drawTile(1,4,7);
    }
    // Bretzn löschen
    LevelGift &bretzel = currentLevel->levelGifts[5];
    if (millis() - bretzel.spawnTime > bretzel.lifetime && bretzel.active) {
        drawTile(1,1,19);
        drawTile(2,1,20);
        drawTile(3,1,14);
    }
    // Bier löschen
    LevelGift &bier = currentLevel->levelGifts[6];
    if (millis() - bier.spawnTime > bier.lifetime && bier.active) {
        drawTile(5,1,19);
    }
    // Wurst löschen
    LevelGift &wurst = currentLevel->levelGifts[4];
    if (millis() - wurst.spawnTime > wurst.lifetime && wurst.active) {
        drawTile(3,4,7);
    }
    // Goldkiste löschen
    LevelGift &goldkiste = currentLevel->levelGifts[7];
    if (millis() - goldkiste.spawnTime > goldkiste.lifetime && goldkiste.active) {
        drawTile(5,4,7);
        drawTile(0,4,7);
    }

}