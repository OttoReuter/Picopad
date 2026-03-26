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
extern int lives;
extern bool jumping;
extern Level* currentLevel;
extern const uint8_t keys;
extern int stop;

bool kaputt = false;
unsigned long rebuilt;

// ===== Level 3 Tiles + Gifts =====
Level level3 = {
    // Tiles
    {
        {15,16,17,17,17,17,17},
        {18,19,20,14,18,19,20},
        { 5, 6, 8, 5, 0, 5, 8},
        { 7, 1, 2, 5, 3, 7, 8},
        { 7, 5, 5, 8, 5, 5, 5},
        {11, 0,11,0, 11,11,0}
    },
    // Gifts
    {
        {DOGPOOP1, 50, 85, false, 0, 4000},
        {DOGPOOP2,120, 85, false, 0, 5000},
        {DOG,     180,85, false, 0, 3000},
        {QUESTION,200,45,false, 0, 5000},
        {WURST,    80,50,false, 0, 4000},
        {BRETZEL,  160,50,false, 0, 4000},
        {BIER,     100,70,false, 0, 8000},
        {GOLDKISTE,140,70,false,0, 6000}
    }
};

// ===== Level 3 Spezial-Logik =====
void updateLevel3() {    
    // Trachtman auf Anfangsposition für Level3
    if (trachtman.y == 85) {
        trachtman.x = 200;
        trachtman.y = 45;
        drawSpriteTransparent(trachtman, t3left, 0xFFFF);
        drawTile(3,2,5);
        movingRight = false;
        movingLeft = false;
    }
    // obere Ebene links und rechtsumkehren
    if (trachtman.y == 45) {
        if(trachtman.x <= 0) {movingLeft=false; movingRight=true;}
        else if(trachtman.x >=260) {movingLeft=true; movingRight=false;}
    }
    // mittlere Ebene links und rechts umkehren
    if (trachtman.y == 125) {
        if(trachtman.x <= 120) {movingLeft=false; movingRight=true;}
        else if(trachtman.x >=260) {movingLeft=true; movingRight=false;}
    }
        
    // Trachtman durch das Rohr nach unten auf mittlere Ebene
    if (trachtman.x > 150 && trachtman.x < 180 && movingLeft == false && movingRight == false) {
        trachtman.x = 164;
        drawTile(3,1,14);
        drawTile(4,1,18);
        drawTile(5,1,19);
        trachtman.y = 125;
        drawSpriteTransparent(trachtman, t3right, 0xFFFF);
    }
    // Trachtman auf Leiter von mittlerer zur unteren Ebene
    if (trachtman.x < 130 && trachtman.y == 125) {
        trachtman.x = 124;
        drawTile(3,3,5);
        trachtman.y = 170;
        movingRight = true;
        movingLeft = false;
        drawSpriteTransparent(trachtman, t3right, 0xFFFF);
    }
    // Trachtman auf Leiter von der unteren Ebene eins hoch
    if (trachtman.x > 110 && trachtman.x < 150 && movingLeft == false && movingRight == false) {
        trachtman.x = 124;
        drawTile(2,4,5);
        drawTile(3,4,8);
        drawTile(4,4,5);
        trachtman.y = 125;
        movingRight = true;
        drawSpriteTransparent(trachtman, t3left, 0xFFFF);
    }
    // Trachtman auf Leiter von der mittleren Ebene ganz hoch
    if (trachtman.x > 214 && trachtman.x < 250 && movingLeft == false && movingRight == false) {
        trachtman.x = 224;
        drawTile(6,3,8);
        drawTile(5,3,7);
        trachtman.y = 45;
        drawSpriteTransparent(trachtman, t3left, 0xFFFF);
        movingRight = true;
    }
    // ================================
    // ======  Brücke stürzt ein  =====
    // ================================
    if (trachtman.x < 30 && trachtman.y == 45 && movingRight && !kaputt) {
        randomSeed(analogRead(29) * (3.3 / 4095) * 3);      // offenen ADC für echtes random
        int einsturz = random (0,3);
        rebuilt = millis();
        if (einsturz > 1){ 
            drawTile(1,2,7);
            trachtman.x = 44;
            trachtman.y = 84;
            kaputt = true;               
        }
    }
    // trachtman links und rechts auf Zwischenebene
    if (trachtman.x >= 120 && trachtman.y == 84) {
        //drawTile(1,2,6);
        movingRight = false;
        movingLeft = true;
    }
    if (trachtman.x <= 65 && trachtman.y == 84 && movingLeft) {
        for(int i=0; i<8; i++) {
            trachtman.y += 12;
            trachtman.x -= 8;
            drawTile(0,2,5);        //fällt schräg nach unten
            drawTile(0,3,7);
            drawTile(1,2,6);
            drawTile(1,3,1);
            drawTile(0,4,7);
            drawSpriteTransparent(trachtman, t3left, 0xFFFF);
        }
        trachtman.y = 170;
    }  
    // Brückenzeit zurücksetzen
    if (millis() - rebuilt > 2500 && kaputt) {
        drawTile(1,2,6);
        kaputt = false;
    }
        
    // trachtman rechts --> Wurstsemmel, Bier  und Goldkiste erscheinen
    if (trachtman.x >= 240 && trachtman.y == 45) {    
        LevelGift &wurst = currentLevel->levelGifts[4];
        if (!wurst.active) {             // nur einmal aktivieren
            wurst.active = true;
            wurst.spawnTime = millis();
            wurst.x = 84;               // feste X-Position
            wurst.y = 41;                // feste Y-Position
        }
        LevelGift &bier = currentLevel->levelGifts[6];
        if (!bier.active) {             // nur einmal aktivieren
            bier.active = true;
            bier.spawnTime = millis();
            bier.x = 124;               // feste X-Position
            bier.y = 84;               // feste Y-Position
        }
        LevelGift &goldkiste = currentLevel->levelGifts[7];
        if (!goldkiste.active) {             // nur einmal aktivieren
            goldkiste.active = true;
            goldkiste.spawnTime = millis();
            goldkiste.x = 204;              // feste X-Position
            goldkiste.y = 81;               // feste Y-Position
        }
    }

    // trachtman links --> Bretzn erscheint
    if (trachtman.x <= 10) {    
        LevelGift &bretzel = currentLevel->levelGifts[5];
        if (!bretzel.active) {             // nur einmal aktivieren
            bretzel.active = true;
            bretzel.spawnTime = millis();
            bretzel.x = 44;                // feste X-Position
            bretzel.y = 166;               // feste Y-Position
        }
        LevelGift &bier = currentLevel->levelGifts[6];
        if (!bier.active) {             // nur einmal aktivieren
            bier.active = true;
            bier.spawnTime = millis();
            bier.x = 200;               // feste X-Position
            bier.y = 124;               // feste Y-Position
        }
        LevelGift &goldkiste = currentLevel->levelGifts[7];
        if (!goldkiste.active) {             // nur einmal aktivieren
            goldkiste.active = true;
            goldkiste.spawnTime = millis();
            goldkiste.x = 204;              // feste X-Position
            goldkiste.y = 166;              // feste Y-Position
        }
    }

    // Dog erscheint in linker Tonne
    if(trachtman.x >= 160 && trachtman.y == 170) {
        LevelGift &dog = currentLevel->levelGifts[2];
        if (!dog.active) {             // nur einmal aktivieren
            dog.active = true;
            dog.spawnTime = millis();
            dog.x = 244;               // Dog auf Tonne rechts
            dog.y = 166;
        }
    }
    
    // =====================================
    // ===== abgelaufene Gifts löschen =====
    // =====================================
    // Dog löschen
    LevelGift &dog = currentLevel->levelGifts[2];
    if (millis() - dog.spawnTime > dog.lifetime && dog.active) {
        dog.active = false;
        if(play){drawTile(6,4,5);}
    }
    // Bretzn löschen
    LevelGift &bretzel = currentLevel->levelGifts[5];
    if (millis() - bretzel.spawnTime > bretzel.lifetime && bretzel.active) {
        if(play){drawTile(1,4,5);}
    }
    // Bier löschen
    LevelGift &bier = currentLevel->levelGifts[6];
    if (millis() - bier.spawnTime > bier.lifetime && bier.active) {
        if(play){
            drawTile(3,2,5);
            drawTile(5,3,7);
        }
    }
    // Wurst löschen
    LevelGift &wurst = currentLevel->levelGifts[4];
    if (millis() - wurst.spawnTime > wurst.lifetime && wurst.active) {
        if(play){drawTile(2,1,20);}
    }
    // Goldkiste löschen
    LevelGift &goldkiste = currentLevel->levelGifts[7];
    if (millis() - goldkiste.spawnTime > goldkiste.lifetime && goldkiste.active) {
        if(play){
            drawTile(5,2,5);
            drawTile(5,4,5);
        }
    }
}