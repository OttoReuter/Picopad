#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include "assets/SpritesBitmap.h"
#include "assets/TmanBitmaps.h"
#include "assets/GiftsBitmaps.h"
#include "engine/Sprite.h"
#include "levels/Level1.h"
#include "levels/Level2.h"
#include "levels/Level3.h"
#include "levels/LevelGifts.h"
#include "config/GameConfig.h"

#define TFT_CS   21
#define TFT_RST  20
#define TFT_DC   17
#define TFT_BLK  16

#define NUM_KEYS 8
// up, down, left, right, X, Y, A, B
const uint8_t keys[NUM_KEYS] = {4, 5, 3, 2, 9, 8, 7, 6};
bool keyHandled[NUM_KEYS] = {false};

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

#define MAP_W 8
#define MAP_H 6

Sprite trachtman = {120, 85};

int points = 0;
int lives = 3;
int surprise = 0;

bool play = false;
bool movingLeft = false;
bool movingRight = false;
bool jumping = false;
bool dropping = false;
bool debounce = false;

int START_X = 120;
int START_Y = 85;
const int droppPower = 12;
const int jumpPower = -12;
const int groundY = 85;

unsigned long lastJump = 0;

// ===== Tiles =====
const uint16_t* tiles[] = {
  b_00,b_01,b_02,b_03,b_04,
  b_05,b_06,b_07,b_08,b_09,
  b_10,b_11,b_12,b_13,b_14,
  b_15,b_16,b_17,b_18,b_19,
  b_20
};
#define NUM_TILES (sizeof(tiles)/sizeof(tiles[0]))

Level* currentLevel = &level1;

// ===== Funktionen =====
void drawTile(int tileX, int tileY, uint8_t tileIndex) {
  if(tileIndex < NUM_TILES) {
    tft.drawRGBBitmap(tileX*40, tileY*40, tiles[tileIndex], 40, 40);
  }
}

void drawLevel(Level* lvl) {
  for(int y=0; y<MAP_H; y++) {
    for(int x=0; x<MAP_W; x++) {
      drawTile(x, y, lvl->tiles[y][x]);
    }
  }
}

void resetPlayer() {
  trachtman.x = START_X;
  trachtman.y = START_Y;
  jumping = false;
  dropping = false;
  movingLeft = false;
  movingRight = false;
  saveSpriteBackground(trachtman);
  drawSpriteTransparent(trachtman, t3left, 0xFFFF);
}

void drawSpriteTransparent(Sprite &s, const uint16_t *bitmap, uint16_t transparentColor) {
  saveSpriteBackground(s);
  for (int16_t j=0;j<32;j++) {
    for(int16_t i=0;i<32;i++) {
      uint16_t color = pgm_read_word(&bitmap[j*32+i]);
      if(color != transparentColor) tft.drawPixel(s.x+i, s.y+j, color);
    }
  }
}

void saveSpriteBackground(Sprite &s) {
    //for(int16_t j = 0; j < SPRITE_HEIGHT; j++) {
    for(int16_t j = 0; j < SPRITE_HEIGHT; j++) {
        //for(int16_t i = 0; i < SPRITE_WIDTH; i++) {
        for(int16_t i = 0; i < SPRITE_WIDTH; i++) {  
            int16_t screenX = s.x + i;
            int16_t screenY = s.y + j;
            uint16_t color = ST77XX_BLACK;
          
            int tileX = screenX / 40;
            int tileY = screenY / 40;
            if(tileX >= 0 && tileX < MAP_W && tileY >= 0 && tileY < MAP_H) {
                uint8_t tileIndex = currentLevel->tiles[tileY][tileX];
                if(tileIndex < NUM_TILES) {
                    const uint16_t* tileBitmap = tiles[tileIndex];
                    int localX = screenX % 40;
                    int localY = screenY % 40;
                    color = pgm_read_word(&tileBitmap[localY*40 + localX]);
                }
            }
            //s.bg[(j + SPRITE_PADDING) * (SPRITE_WIDTH + 2*SPRITE_PADDING) + (i + SPRITE_PADDING)] = color;
            s.bg[j * SPRITE_WIDTH + i] = color;
        }
    }
}

void restoreSpriteBackground(Sprite &s) {
    //tft.drawRGBBitmap(s.x - SPRITE_PADDING, s.y - SPRITE_PADDING, s.bg,
    tft.drawRGBBitmap(s.x, s.y, s.bg,
                  //SPRITE_WIDTH + 2*SPRITE_PADDING,
                  SPRITE_WIDTH,
                  //SPRITE_HEIGHT + 2*SPRITE_PADDING);
                  SPRITE_HEIGHT);
}

// Level updaten
void updateCurrentLevel() {
    if(currentLevel == &level1) {
        updateLevel1();
    }
    else if(currentLevel == &level2) {
        updateLevel2();
    }
    else if(currentLevel == &level3) {
        updateLevel3();
    }
}

// Automatischer Wechsel des Level
void checkLevelProgress() {
    if(currentLevel == &level1 && points >= 300) {
      currentLevel = &level2;
      drawLevel(currentLevel);
      resetPlayer();
    }
    else if(currentLevel == &level2 && points >= 600) {
      currentLevel = &level3;
      drawLevel(currentLevel);
      resetPlayer();
    }  
    else if (currentLevel == &level3 && points >= 1000) {
      lives = 0;
    }
    if (lives == 0) {
      currentLevel = &level1;
      points = 0;
      lives = 3;
      play = false;
      movingLeft = false;
      movingRight = false;
      startGame();
    }
}

void readInput() {
  // Start && Reset Game - Taste B
  if(!digitalRead(keys[7])) {
    play = true;
    drawLevel(currentLevel);
    resetPlayer();
    // zum Testen der Level
    //points = 300;   // 300 Level 2, 600 Level 3
  }
  // wenn play == true
  if (play) {
    // up, down, left, right, X - stop,
    // [2] left; [3] right; [4] Taste X;
    if(!digitalRead(keys[2])) { movingLeft=true; movingRight=false;}
    else if(!digitalRead(keys[3])) { movingRight=true; movingLeft=false; }
    else if(!digitalRead(keys[4])) {
      if(movingLeft) drawSpriteTransparent(trachtman, t3left, 0xFFFF);
      else if(movingRight) drawSpriteTransparent(trachtman, t3right,0xFFFF);
      movingLeft=false; movingRight=false;
    }
    // nach oben springen - Taste Y
    if(!digitalRead(keys[5]) && !debounce) {
      debounce=true; lastJump=millis();
      restoreSpriteBackground(trachtman);
      trachtman.y+=2*jumpPower;
      drawSpriteTransparent(trachtman,t1left,0xFFFF);
    }
    if(millis()-lastJump>200 && debounce) {
      restoreSpriteBackground(trachtman);
      trachtman.y-=2*jumpPower;
      drawSpriteTransparent(trachtman,t1left,0xFFFF);
      debounce=false;
    }
  }
  // hoch oder runter gehen nur auf der oberen Ebene
  if (trachtman.y <= 85) {
    if(!digitalRead(keys[0]) && !jumping) {
      jumping=true;
      trachtman.y+=jumpPower;
      jumping=false;
      if (trachtman.y < 0) {trachtman.y = 8;}
    }
    if(!digitalRead(keys[1]) && !dropping) {
      dropping=true;
      trachtman.y+=droppPower;
      dropping=false;
      trachtman.y -= 12;
      if (trachtman.y > 85) {trachtman.y = 85;}
      }
  }
}

// ===== Animation =====
void animateMovement() {
  const uint16_t* leftFrames[] = {t1left,t2left,t3left,t4left};
  const uint16_t* rightFrames[] = {t1right,t2right,t3right,t4right};
  const uint16_t** frames = movingLeft ? leftFrames : rightFrames;
  
  for(int i=0;i<4;i++) {
    drawSpriteTransparent(trachtman, frames[i], 0xFFFF);
    delay(50);
    restoreSpriteBackground(trachtman);
    trachtman.x += movingLeft?-5:5;
  }
}

void drawHUD() {
  // Hintergrund für HUD löschen
  tft.fillRect(80, 0, 150, 45, 0x05F7);
  // Punkte anzeigen
  tft.setCursor(80, 5);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.print("Points: ");
  tft.print(points);
  // Leben anzeigen
  tft.setCursor(93, 25);
  tft.setTextColor(ST77XX_BLUE);
  tft.print("Lives: ");
  tft.print(lives);
}

// Start Game
void startGame() {
  tft.fillScreen(0x4000);
  tft.setTextSize(4);
  tft.setCursor(55,15);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("Trachtman");
  tft.setCursor(53,17);
  tft.setTextColor(ST77XX_RED);
  tft.print("Trachtman");
  tft.setTextSize(2);
  tft.setCursor(160,65);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("is back");
  tft.setCursor(75,220);
  tft.print("press B button");
  tft.drawRGBBitmap(80, 80, intro, 140, 120);
  tft.setTextSize(1);
  tft.setCursor(250,190);
  tft.print("V.1.10");
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  pinMode(TFT_RST, OUTPUT); digitalWrite(TFT_RST, LOW); delay(50); digitalWrite(TFT_RST,HIGH);
  tft.init(240,320); tft.setRotation(3);
  pinMode(TFT_BLK,OUTPUT); digitalWrite(TFT_BLK,HIGH);
  //pinMode(14, OUTPUT);    // rote LED
  //pinMode(13, OUTPUT);    // gelbe LED
  //pinMode(12, OUTPUT);    // grüne LED
  startGame();
 
  for(int i=0;i<NUM_KEYS;i++) pinMode(keys[i],INPUT_PULLUP);
}

// ===== Loop =====
void loop() {
    // 1. Player Input
    readInput();
    // 2. Level-Logik
    updateCurrentLevel();
    // 3. Bewegung + Randprüfung
    handlePlayerMovement();
    // 4. Gifts aktualisieren (Lebenszeit)
    updateLevelGifts(*currentLevel);
    // 5. Alle aktiven Gifts zeichnen
    for(int i=0;i<8;i++) { drawGift(currentLevel->levelGifts[i]);}
    // 6. Kollision prüfen
    checkLevelGiftCollisions(trachtman, *currentLevel);
    // 7. HUD (Punkte und Leben)
    if (play) drawHUD();
    // 8. Animation von Trachtman
    if(movingLeft || movingRight) animateMovement();
    // 9. Levelcheck
    checkLevelProgress();

    delay(20);
}
