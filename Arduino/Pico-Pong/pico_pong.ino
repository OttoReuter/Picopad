#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Fonts/FreeSansBold12pt7b.h>

#include "config/GameConfig.h"

#define TFT_CS   21
#define TFT_RST  20
#define TFT_DC   17
#define TFT_BLK  16

#define NUM_KEYS 8
// up, down, left, right, X, Y , A, B
const uint8_t keys[NUM_KEYS] = {4, 5, 3, 2, 9, 8, 7, 6};
bool keyHandled[NUM_KEYS] = {false};

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// ---------------- GAME STATE ----------------
int width = 320;
int height = 240;
int score1 = 0;
int score2 = 0;
bool run = false;
bool play = false;
bool debounce = false;
bool up = false;
bool down = false;
unsigned long lastPush = 0;
// Player
int player_x = 20;
//int player_y = 100;
int player_y = width/2-20;
int player_y_dif = 4;
// Pico
//int pico_x = 255;
//int pico_y = 100;
int pico_x = width-25;
int pico_y = width/2-20;
// Ball
//int ball_x = 140;
//int ball_y = 120;
int ball_x = width/2;
int ball_y = height/2;
int ball_dir_x = 3;
int ball_dir_y = 3;

void reset_ball() {
  ball_x = int(width/2);
  randomSeed(analogRead(29) * (3.3 / 4095) * 3);
  ball_y = random(30, 230);
  randomSeed(analogRead(29) * (3.3 / 4095) * 3);
  ball_dir_y = random(2, 4);
  if (random(0, 1)) {
    ball_dir_x = -3;
  }
  else {
    ball_dir_x = 3;
    ball_dir_y *= -1;
  }
}

void update_score() {
  if (score1 == 10 or score2 == 10) {
    //score1=6;score2=10;
    game_over();
  } 
  tft.fillRect(75,8,200,30,0x4000);
  tft.setTextSize(3);
  tft.setCursor(100,10);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print(score1);
  tft.print("  :  ");
  tft.print(score2);
  tft.drawRect(19,39,282,180,ST77XX_WHITE);
  reset_ball();
  delay(200);
}

void calc_ball() {
  Serial.println(ball_dir_x);
  tft.fillCircle(ball_x,ball_y,4,0x4000);
  // Wand oben/unten
  if (ball_y <= 46 or ball_y >= height - 40) {
    ball_dir_y *= -1;
  }
  // Links (Player Seite)
  if (ball_dir_x > 0) {
    if (ball_x <= 10) {
      score2 += 1;
      update_score();
    }
    if (ball_x <= 36 && (player_y <= ball_y && ball_y <= player_y +40 )) {
      ball_dir_x *= -1;
    }
  }
  // Rechts (Pico Seite)
  else {
    if (ball_x >= 290 && (pico_y <= ball_y && ball_y <= pico_y + 40)) {
      ball_dir_x *= -1;
    }
    if (ball_x >= 295) {
      score1 += 1;
      update_score();
    }
  }
  ball_x -= ball_dir_x;
  ball_y += ball_dir_y;
  tft.fillCircle(ball_x,ball_y,4,ST77XX_WHITE);
}

void set_player() {
  if (player_y >= 41 && up) {
    tft.fillRect(player_x,player_y,5,40,0x4000);
    player_y = player_y - player_y_dif;
    tft.fillRect(player_x,player_y,5,40,ST77XX_WHITE);
  }
  if (player_y <= 175 && down) {
    tft.fillRect(player_x,player_y,5,40,0x4000);
    player_y = player_y + player_y_dif;
    tft.fillRect(player_x,player_y,5,40,ST77XX_WHITE);
  }
}

void calc_pico() {
  tft.fillRect(pico_x,pico_y,5,40,0x4000);
  if (pico_y + 20 < ball_y) {
    pico_y += 3;
  }
  else {
    if (pico_y + 20 > ball_y) {
      pico_y -= 3;
    }
  }
  pico_y = max(40, min(pico_y, height - 65));
  tft.fillRect(pico_x,pico_y,5,40,ST77XX_WHITE);
}

void readInput() {
  // Spiel starten A Button
  if(!digitalRead(keys[6]) && !debounce) {
    play = true;debounce = true;lastPush = millis();
    tft.fillRect(55,95,170,50,0x4000);
    tft.fillRect(60,175,200,40,0x4000);
  }
  // New Game B Button
  if(!digitalRead(keys[7]) && !debounce && !play) {
    debounce = true;lastPush = millis();
    score1 = 0;score2 = 0;
    startGame();
  }
  // wenn play == true
  if (play) {
    // Spiel unterbrechen 'X' oder 'Y'
    if(!digitalRead(keys[4]) or !digitalRead(keys[5])) {
      play=false;
      tft.drawRect(19,39,282,180,ST77XX_WHITE);
    }
    // nach oben
    if(!digitalRead(keys[0])) {
      up = true;
      down = false;
      set_player();
    }
    // nach unten
    if(!digitalRead(keys[1])) {
      down = true;
      up = false;
      set_player();
    }
  }
  if(millis()-lastPush>400 && debounce) {
    debounce=false;
  }
}

// Start Game
void startGame() {
  tft.fillScreen(0x4000);
  tft.drawRect(19,39,282,180,ST77XX_WHITE);
  tft.setTextSize(3);
  tft.setCursor(100,10);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("PICO-PONG");
  tft.setTextSize(2);
  tft.setCursor(65,180);
  tft.print("Start: A  Button");
  tft.fillRect(player_x,player_y,5,40,ST77XX_WHITE);
  tft.fillRect(pico_x,pico_y,5,40,ST77XX_WHITE);
  tft.fillCircle(ball_x,ball_y,4,ST77XX_WHITE);
}

void game_over() {
  play=false;
  tft.drawRect(19,39,282,180,ST77XX_WHITE);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(80,100);
  tft.print("GAME OVER");
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
  for(int i=0;i<NUM_KEYS;i++) pinMode(keys[i],INPUT_PULLUP);
  startGame();
}

// ===== Loop =====
void loop() {
  // Tasten abfragen
  readInput();
  // Spiel läuft
  if (play){
    calc_ball();
    calc_pico();
    delay(25);
  }
}
