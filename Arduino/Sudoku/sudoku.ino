#include <SPI.h>
#include "Display.h"
#include "Generator.h"
#include "Solver.h"
#include "Game.h"
#include "GameTypes.h"
#include "Masks_leicht.h"
#include "Masks_mittel.h"
#include "Masks_schwer.h"

#define TFT_CS   21
#define TFT_RST  20
#define TFT_DC   17
#define TFT_BLK  16

#define NUM_KEYS 8
// up, down, left, right, Y, A, B, X
const uint8_t keys[NUM_KEYS] = {4, 5, 3, 2, 8, 7, 6, 9};
bool keyHandled[NUM_KEYS] = {false};

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// ---------------- STATE ----------------
bool debounce = false;
bool mask_verified= false;
bool isready = false;
GameState state = MENU;
unsigned long lastPress = 0;
unsigned long startzeit = 0;
unsigned long gemessene_Zeit = 0;
int i;
int j;
int menuIndex = 1;
int solveIndex = 1;
int level;
int count;
int ma, mm, ms;
String levelstring;
int x_pos[] = {4,30,55,80,106,131,156,182,207};
int y_pos[] = {4,30,55,80,106,131,156,182,207};
int zahl;
int zahl1;
int zahl2;
int points = 0;
int failed = 0;
// Ausgangsfeld, welches den Regeln entspricht
int baseArray[81] = {1,2,3,4,5,6,7,8,9,4,5,6,7,8,9,1,2,3,7,8,9,1,2,3,4,5,6,
                2,3,4,5,6,7,8,9,1,5,6,7,8,9,1,2,3,4,8,9,1,2,3,4,5,6,7,
                3,4,5,6,7,8,9,1,2,6,7,8,9,1,2,3,4,5,9,1,2,3,4,5,6,7,8};
//const uint8_t array[81];
int array[81];
int result[81];

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  pinMode(TFT_RST, OUTPUT); digitalWrite(TFT_RST, LOW); delay(50); digitalWrite(TFT_RST,HIGH);
  tft.init(240, 320); tft.setRotation(3);
  pinMode(TFT_BLK,OUTPUT); digitalWrite(TFT_BLK,HIGH);
  
  for(int i=0;i<NUM_KEYS;i++) pinMode(keys[i],INPUT_PULLUP);

  randomSeed(analogRead(28));
     
  drawMenu();
}

// ================
// ===== Loop =====
// ================
void loop() {
  // ==========
  // Button UP      
  if (!digitalRead(keys[0]) && !debounce) {
    switch(state) {
      case MENU:
        menuIndex--;
        if(menuIndex<1) menuIndex=3;
        drawMenu();
        break;
      case SELECT_POSITION:
        tft.drawRect(2, 2+j*26,234,26,0x0320);
        tft.drawRect(2+i*26, 2, 26, 234,0x0320);
        tft.drawRect(2+i*26,2+j*26,26,26,0x0320);
        draw_gitter();
        j--;
        if(j<0)
          j=8;
        select_position();
        break;
      case SELECT_NUMBER:
        solveIndex--;
        if(solveIndex<1)
          solveIndex=9;
        drawSideboard();
        break;
    }
    debounce = true; lastPress = millis();
  }
  // ===========
  // Button DOWN
  if (!digitalRead(keys[1]) && !debounce) {
    switch(state) {
      case MENU:
        menuIndex++;
        if(menuIndex>3) menuIndex=1;
        drawMenu();
        break;
      case SELECT_POSITION:
        tft.drawRect(2, 2+j*26,234,26,0x0320);
        tft.drawRect(2+i*26, 2, 26, 234,0x0320);
        tft.drawRect(2+i*26,2+j*26,26,26,0x0320);
        draw_gitter();
        j++;
        if(j>8)
          j=0;
        select_position();
        break;
      case SELECT_NUMBER:
        solveIndex++;
        if(solveIndex>9)
          solveIndex=1;
        drawSideboard();
        break;
    }
    debounce = true; lastPress = millis();
  }
  // ===========
  // Button LEFT
  if (!digitalRead(keys[2]) && !debounce) {
    if(state==SELECT_POSITION) {
      tft.drawRect(2, 2+j*26,234,26,0x0320);
      tft.drawRect(2+i*26, 2, 26, 234,0x0320);
      tft.drawRect(2+i*26,2+j*26,26,26,0x0320);
      draw_gitter();
      i--;
      if(i<0)
        i=8;
    select_position();
    }
    debounce = true; lastPress = millis();
  }
  // ===========
  // Button RIGHT
  if (!digitalRead(keys[3]) && !debounce) {
    if(state==SELECT_POSITION) {
      tft.drawRect(2, 2+j*26,234,26,0x0320);
      tft.drawRect(2+i*26, 2, 26, 234,0x0320);
      tft.drawRect(2+i*26,2+j*26,26,26,0x0320);
      draw_gitter();
      i++;
      if(i>8)
        i=0;
    select_position();
    }
    debounce = true; lastPress = millis();
  }

  // Sudoku starten mit A-Button
  if (!digitalRead(keys[5]) && !debounce) {
    if (!mask_verified) {   // Spiel in der gewählten Schwierigkeit starten
      debounce = true; lastPress = millis();
      tft.fillScreen(0x4000);
      if (menuIndex == 1) {
        level = 1;
        levelstring = "leicht";
      } else if (menuIndex == 2) {
          level = 2;
          levelstring = "mittel"; ;
      } else if (menuIndex == 3) {
          level = 3;
          levelstring = "schwer";
      }
      startzeit = millis();
      start_sudoku();
    }
  }

  // zurueck zum Menu mit B-Button
  if (!digitalRead(keys[6]) && !debounce) {
    // Abbruch und Rückkehr zum Menü
    state = MENU;
    mask_verified = false;
    points = 0;
    failed = 0;
    isready = false;
    drawMenu();
    debounce = true; lastPress = millis();
  }

  // Taste OK bei Zahlenauswahl gedrueckt
  if(!digitalRead(keys[4]) && !debounce) {
    switch(state) {
        case SELECT_POSITION:
            if(array[j*9+i]==0) {
              solveIndex = 1;
              state = SELECT_NUMBER;
              drawSideboard();
            }
            break;
        case SELECT_NUMBER:
            select_number();
            tft.fillRect(240,5,75,230,0x4000);
            state = SELECT_POSITION;
            select_position();
            break;
    }
    debounce = true;
    lastPress = millis();
  }
  if (debounce) {
    bool released = true;
    for (int k = 0; k < NUM_KEYS; k++) {
        if (!digitalRead(keys[k])) {
            released = false;
            break;
        }
    }
    if (released && millis() - lastPress > 200) {
        debounce = false;
    }
  }
}
