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
extern Level* currentLevel;
extern bool movingLeft, movingRight;
extern bool play;
extern int lives;
extern void startGame();

int position = 0;

// ===== Level 1 Tiles + Gifts =====
Level level1 = {
    // Tiles
    {
        {15,16,13,13,13,13,13},
        {18,19,20,14,18,19,20},
        { 7, 7, 7, 7, 7, 7, 7},
        { 7, 0, 5, 5, 5, 8, 6},
        { 7, 3, 7, 7, 7, 7, 2},
        {12, 0,11,11, 8,10,11}
    },
    // Gifts
    {
        {DOGPOOP1, 50, 85, false, 0, 4000},
        {DOGPOOP2,120, 85, false, 0, 3000},
        {DOG,     180,85, false, 0, 2000},
        {QUESTION,200,45,false, 0, 5000},
        {WURST,    80,50,false, 0, 4000},
        {BRETZEL,  160,50,false, 0, 5000},
        {BIER,     100,70,false, 0, 4000},
        {GOLDKISTE,140,70,false,0, 8000}
    }
};

// ===== Level 1 Spezial-Logik =====
void updateLevel1() {
    // Trachtman auf der Leiter zur unteren Ebene
    if (trachtman.x > 180 && trachtman.x < 210 && movingLeft == false && movingRight == false) {
        trachtman.x = 174;
        trachtman.y = 170;
        drawTile(5,2,7);
        drawTile(4,2,7);
        movingLeft = true;
        // Bretzel erscheint
        LevelGift &bretzel = currentLevel->levelGifts[5];
        if (!bretzel.active) {             // Bretzel aktivieren
            bretzel.active = true;
            bretzel.spawnTime = millis();
            bretzel.x = 84;                // feste Position
            bretzel.y = 130;
        }
        // Bier erscheint
        LevelGift &bier = currentLevel->levelGifts[6];
        if (!bier.active) {                // Bier aktivieren
            bier.active = true;
            bier.spawnTime = millis();
            bier.x = 8;                // feste Position
            bier.y = 166;
        }
    }
    // Trachtman läuft unten nach rechts
    if (trachtman.y >= 85 && trachtman.x >= 160 && movingRight) {
        // Goldkiste erscheint
        LevelGift &goldkiste = currentLevel->levelGifts[7];
        if (!goldkiste.active) {             // Goldkiste aktivieren
            goldkiste.active = true;
            goldkiste.spawnTime = millis();
            goldkiste.x = 84;                // feste Position
            goldkiste.y = 170;
        }
    }
    // Trachtman fällt in den Keller
    if(trachtman.x < 40 && trachtman.y > 80) {
        // Goldkiste erscheint
        LevelGift &goldkiste = currentLevel->levelGifts[7];
        if (!goldkiste.active) {             // Goldkiste aktivieren
            goldkiste.active = true;
            goldkiste.spawnTime = millis();
            goldkiste.x = 44;                // feste Position
            goldkiste.y = 44;
        }
        trachtman.y = 170;
        drawTile(6,0,17);    // rechte obere Ecke löschen
        drawTile(1,4,3);     // Tor auf
        drawTile(0,3,7);
        drawTile(0,5,12);
        // Fragezeichen erscheint in unterer oder oberer Ebene
        LevelGift &question = currentLevel->levelGifts[3];
        if (!question.active) {             // nur einmal aktivieren
            question.active = true;
            question.spawnTime = millis();
            question.x = 122;                // Fragezeichen unten
            question.y = 130;
            randomSeed(analogRead(29) * (3.3 / 4095) * 3);      // offenen ADC für echtes random
            int position = random (0,3);
            if (position > 0){ 
                int position_x = 120 + 40*position;
                question.x = position_x;      // Fragezeichen oben, variable
                question.y = 50;
            }
        }
    }
    // Tor schließt sich
    if(trachtman.y == 170 && trachtman.x > 60) {
        if (play) {drawTile(1,4,0);}
    }
    // Trachtman klettert rechts hoch
    if(trachtman.y == 170 && trachtman.x > 205) {
        trachtman.y = 85;
        drawTile(4,4,7);
        drawTile(5,4,7);
        drawTile(6,4,2);
        // DOGPOOP1 aktivieren
        if (trachtman.x > 200) {    
            LevelGift &dogpoop1 = currentLevel->levelGifts[0];
            if (!dogpoop1.active) {             // nur einmal aktivieren
                dogpoop1.active = true;
                dogpoop1.spawnTime = millis();
                randomSeed(analogRead(29) * (3.3 / 4095) * 3);   // offenen ADC für echtes random
                int dogpoop_x = 40 + 40*random (0,3);
                dogpoop1.x = dogpoop_x;
                dogpoop1.y = 85;                // feste Y-Position
            }
        }
    }
    // DOGPOOP2 aktivieren
        if (trachtman.x <= 0 || trachtman.x >= 240 ) {    
            LevelGift &dogpoop2 = currentLevel->levelGifts[1];
            if (!dogpoop2.active) {             // nur einmal aktivieren
                dogpoop2.active = true;
                dogpoop2.spawnTime = millis();
                dogpoop2.x = 160;               // feste X-Position
                dogpoop2.y = 85;                // feste Y-Position
            }
        }
    
    // ==============================
    // ==== abgelaufene Gifts =======
    // ==============================
    // dogpoop1 löschen
    LevelGift &dogpoop1 = currentLevel->levelGifts[0];
    if (millis() - dogpoop1.spawnTime > dogpoop1.lifetime && dogpoop1.active) {
        drawTile(0,2,7);
        drawTile(1,2,7);
        drawTile(2,2,7);
    }
    // dogpoop2 löschen
    LevelGift &dogpoop2 = currentLevel->levelGifts[1];
    if (millis() - dogpoop2.spawnTime > dogpoop2.lifetime && dogpoop2.active) {
        drawTile(4,2,7); 
    }
    // Goldkiste löschen
    LevelGift &goldkiste = currentLevel->levelGifts[7];
    if (millis() - goldkiste.spawnTime > goldkiste.lifetime && goldkiste.active) {
        drawTile(1,1,19);
        drawTile(2,4,7);
    }
    // Bretzel löschen
    LevelGift &bretzel = currentLevel->levelGifts[5];
    if (millis() - bretzel.spawnTime > bretzel.lifetime && bretzel.active) {
        drawTile(2,3,5);
    }
    // Bier löschen
    LevelGift &bier = currentLevel->levelGifts[6];
    if (millis() - bier.spawnTime > bier.lifetime && bier.active) {
        drawTile(0,4,7);
    }
    // Fragezeichen löschen
    LevelGift &question = currentLevel->levelGifts[3];
    if (millis() - question.spawnTime > question.lifetime && question.active) {
        drawTile(3,3,5);                           // Fragezeichen unten
        drawTile(3,1,14);                          // Fragezeichen oben
        drawTile(4,1,18);                          // 3x, weil variable
        drawTile(5,1,19);    
    }
}