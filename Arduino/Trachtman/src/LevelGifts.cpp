#include "../levels/LevelGifts.h"   // eigener Header
#include "../levels/Level1.h"       // enthält Level + LevelGift Strukturen und DOGPOOP1..GOLDKISTE
#include "../levels/Level2.h"
#include "../levels/Level3.h"
#include "../engine/Sprite.h"       // enthält Sprite Definition
#include "../assets/GiftsBitmaps.h" // gift_1 .. gift_8
#include "../assets/SpritesBitmap.h"
#include "../assets/TmanBitmaps.h"
#include <Arduino.h>      // für millis()
#include <Adafruit_ST7789.h> // tft
#include "../engine/SpriteDraw.h"
#include "../config/GameConfig.h"

// Externe globale Variablen aus main.ino
extern Level* currentLevel;
extern Sprite trachtman;
extern bool movingLeft, movingRight;
extern Adafruit_ST7789 tft;
extern int points;
extern bool play;
extern int lives;
extern int surprise;
extern void drawSpriteTransparentXY(int x, int y, const uint16_t *bitmap, uint16_t transparentColor);
extern void drawTile(int tileX, int tileY, uint8_t tileIndex);

// Mapping von Gift-Typ zu Bitmap
const uint16_t* getGiftBitmap(uint8_t type) {
    switch(type) {
        case DOGPOOP1: return gift_1;
        case DOGPOOP2: return gift_2;
        case DOG:      return gift_3;
        case QUESTION: return gift_4;
        case WURST:    return gift_5;
        case BRETZEL:  return gift_6;
        case BIER:     return gift_7;
        case GOLDKISTE:return gift_8;
    }
    return nullptr;
}

// ===== Player Bewegung & Randumkehrung =====
void handlePlayerMovement() {
    if(movingLeft) trachtman.x -= 5;
    if(movingRight) trachtman.x += 5;

    // Bildschirmränder
    if(trachtman.x <= 0) { movingLeft=false; movingRight=true; }
    if(trachtman.x >= 280) { movingLeft=true; movingRight=false; }
    // Level1
    if(currentLevel == &level1) {
        if(trachtman.y < 85 && trachtman.x >= 280) {
            trachtman.y = 85;
        }
        if (trachtman.y < 80 && trachtman.x <= 0) {
            trachtman.x = 41;
            trachtman.y = 85;
            movingLeft = false;
            movingRight = true;
        }
    }
    // Level 2
    if(currentLevel == &level2) {
        if (trachtman.y <85 && (trachtman.x <= 0 || trachtman.x >= 280)) {
            trachtman.y = 85;
        }
    }
    // Level 3
    if(currentLevel == &level3) {
        if (trachtman.y <45 && (trachtman.x <= 0 || trachtman.x >= 280)) {
            trachtman.y = 45;
        }
    }
}

// ===== Gifts nach Lebenszeit entfernen =====
void updateLevelGifts(Level &lvl) {
    unsigned long now = millis();
    for(int i=0; i<8; i++) {
        LevelGift &g = lvl.levelGifts[i];
        if(g.active && now - g.spawnTime > g.lifetime) {
            g.active = false;  
        }
    }
}

void drawGift(LevelGift &g) {
    if(!g.active) return;

    const uint16_t* bmp = getGiftBitmap(g.type);
    if(bmp)
        drawSpriteTransparentXY(g.x, g.y, bmp, 0xFFFF);
}

// ================================================
// ===   Kollision Trachtman <-> Gifts prüfen   ===
// ================================================
void checkLevelGiftCollisions(Sprite &player, Level &lvl) {
    const int giftW = 16;
    const int giftH = 16;
    const int tolerance = 4; // großzügiger Bereich
    randomSeed(analogRead(29) * (3.3 / 4095) * 3);      // offenen ADC für echtes random
    int surprise = random (0,3);                        // Zufall für Fragezeichen
    
    for(int i=0; i<8; i++) {
        LevelGift &g = lvl.levelGifts[i];
        if(!g.active) continue;

        // Rechteck-Kollision mit Toleranz
        if(player.x + 32 > g.x - tolerance && player.x < g.x + giftW + tolerance &&
           player.y + 32 > g.y - tolerance && player.y < g.y + giftH + tolerance) {

            // Gift deaktivieren + Hintergrund wiederherstellen
            g.active = false;
            drawTile(g.x / 40, g.y / 40, lvl.tiles[g.y / 40][g.x / 40]);

            // Punkte / Leben anpassen
            switch(g.type) {
                case QUESTION:
                    // egal ob seitlich oder von unten → Punkte
                    if (surprise == 0) {           // Bretzel
                        drawSpriteTransparentXY(240, 8, gift_6, 0xFFFF);
                        points += 5;
                    }
                    if (surprise == 1) {           // Bier
                        drawSpriteTransparentXY(240, 8, gift_7, 0xFFFF);
                        points += 15;                  
                    }
                    else if (surprise == 2) {       // Weisswurst
                        drawSpriteTransparentXY(240, 8, gift_5, 0xFFFF);
                        points += 5;                  
                    }
                    break;
                case DOGPOOP1:
                    // großzügige Überspring-Prüfung
                    if(player.y + 32 < g.y + giftH / 2) {
                        points += 5; // erfolgreich übersprungen
                    } else {
                        points = max(0, points - 5); // drauf gelaufen
                        for(int i=0; i<8; i++) {
                            digitalWrite(14, HIGH);
                            delay(15);
                            digitalWrite(14, LOW);
                            delay(15);
                        }
                    }
                    break;
                case DOGPOOP2:
                    // großzügige Überspring-Prüfung
                    if(player.y + 32 < g.y + giftH / 2) {
                        points += 5; // erfolgreich übersprungen
                    } else {
                        points = max(0, points - 5); // drauf gelaufen
                        for(int i=0; i<8; i++) {
                            digitalWrite(14, HIGH);
                            delay(15);
                            digitalWrite(14, LOW);
                            delay(15);
                        }
                    }
                    break;
                case DOG:
                    // Dog berühren
                    if (points >= 300 && points < 600) {       // Dog in Level 2
                        if(player.x - 40 < g.x + 30) {
                            lives -= 1; // Leben abziehen
                            for(int i=0; i<12; i++) {
                                digitalWrite(14, HIGH);
                                tft.drawRGBBitmap(40, 161, stern1, 40, 40);
                                delay(30);
                                digitalWrite(14, LOW);
                                tft.drawRGBBitmap(40, 161, stern2, 40, 40);
                                delay(30);
                            }
                            trachtman.x = 120;
                            trachtman.y = 85;
                            drawSpriteTransparentXY(120, 85, t3right, 0xFFFF);
                            drawTile(1,4,7);          // Hintergrund Dog
                            drawTile(4,3,6);          // Brücke restore
                        }
                    }
                    if (points >= 600) {       // Dog in Level 3
                        if(player.x - 40 < g.x + 30) {
                            lives -= 1; // Leben abziehen
                            for(int i=0; i<12; i++) {
                                digitalWrite(14, HIGH);
                                tft.drawRGBBitmap(244, 161, stern1, 40, 40);
                                delay(30);
                                digitalWrite(14, LOW);
                                tft.drawRGBBitmap(244, 161, stern2, 40, 40);
                                delay(30);
                            }
                            trachtman.x = 200;
                            trachtman.y = 45;
                            drawSpriteTransparentXY(200, 45, t3left, 0xFFFF);
                            drawTile(6,4,5);          // Hintergrund Dog
                            movingRight = false;
                            movingLeft = false;
                        }
                    }
                    if (lives == 0){
                        tft.setTextSize(3);
                        tft.fillRect(0, 80, 280, 40, 0x4000);
                        tft.setCursor(7,85);
                        tft.setTextColor(ST77XX_YELLOW);
                        tft.print("Neustart in 10s");
                        tft.setCursor(6,86);
                        tft.setTextColor(ST77XX_RED);
                        tft.print("Neustart in 10s");
                        delay(10000);
                        rp2040.reboot();
                    }
                    break;
                case WURST:
                    // egal ob seitlich oder von unten → Punkte
                    points += 10;
                    break;
                case BRETZEL:
                    // egal ob seitlich oder von unten → Punkte
                    points += 10;
                    break;
                case BIER:
                    // egal ob seitlich oder von unten → Punkte
                    points += 20;
                    break;
                case GOLDKISTE:
                    points += 25;
                    break;
            }
        }
    }
}