#include "Display.h"
#include <Adafruit_ST7789.h>

// Gitterlinien zeichnen
void draw_gitter() {
  tft.drawRect(1,1,235,235,ST77XX_WHITE);
  tft.drawRect(2,2,233,233,ST77XX_WHITE);
  // senkrechte Linien
  tft.drawLine(27,4,27,232,0xA514);
  tft.drawLine(53,4,53,232,0xA514);
  tft.drawLine(78,4,78,232,ST77XX_WHITE);
  tft.drawLine(79,4,79,232,0xA514);
  tft.drawLine(80,4,80,232,ST77XX_WHITE);
  tft.drawLine(105,4,105,232,0xA514);
  tft.drawLine(131,4,131,232,0xA514);
  tft.drawLine(154,4,154,232,ST77XX_WHITE);
  tft.drawLine(155,4,155,232,0xA514);
  tft.drawLine(156,4,156,232,ST77XX_WHITE);
  tft.drawLine(181,4,181,232,0xA514);
  tft.drawLine(207,4,207,232,0xA514);
  //waagerechte Linien
  tft.drawLine(4,27,232,27,0xA514);
  tft.drawLine(4,53,232,53,0xA514);
  tft.drawLine(4,78,232,78,ST77XX_WHITE);
  tft.drawLine(4,79,232,79,0xA514);
  tft.drawLine(4,80,232,80,ST77XX_WHITE);
  tft.drawLine(4,105,232,105,0xA514);
  tft.drawLine(4,131,232,131,0xA514);
  tft.drawLine(4,156,232,156,ST77XX_WHITE);
  tft.drawLine(4,157,232,157,0xA514);
  tft.drawLine(4,158,232,158,ST77XX_WHITE);
  tft.drawLine(4,183,232,183,0xA514);
  tft.drawLine(4,209,232,209,0xA514);
}

// Clear Screen
void clear_screen() {
  tft.fillScreen(0x4000);
  tft.fillRect(1,1,235,235,0x0320);
  draw_gitter();
}

// Array anzeigen
void array_show() {
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  for (j=0;j<9;j++){
    for (i=0;i<9;i++){
      if (array[j*9+i]== 0){
        tft.setTextColor(0x0320);
      } else {
        tft.setTextColor(ST77XX_WHITE);
      }
      tft.setCursor(x_pos[i]+8,y_pos[j]+5);
      if ((j+i+1)<10) {
        tft.print(array[j*9+i]);
      } else {
        tft.print(array[j*9+i]);
      }
    }
  }
}

// ==================================
// ==========Start MENU =============
// ==================================
void drawMenu() {
  tft.fillScreen(0x4000);
  tft.setTextSize(2);
  tft.setCursor(40, 20);
  tft.setTextColor(ST77XX_WHITE, 0x4000);
  tft.print("Sudoku Mini-Console");
  tft.setCursor(20,190);
  tft.print("Confirm: A - Button");
  tft.setCursor(20,220);
  tft.print("New S. : B - Button");

  drawMenuItem(1, "Sudoku leicht", 60);
  drawMenuItem(2, "Sudoku mittel", 100);
  drawMenuItem(3, "Sudoku schwer", 140);
}
void drawMenuItem(int index, const char* text, int y) {
  if (menuIndex == index) {
    tft.setTextColor(ST77XX_YELLOW, 0x4000);
    tft.setCursor(20,y);tft.print("*");
  } else {
    tft.setTextColor(ST77XX_WHITE, 0x4000);
    tft.setCursor(20,y);tft.print(" ");
  }
  tft.setCursor(60, y);tft.print(text);
}

void drawSideboard() {
  tft.fillRect(240,5,75,230,0x4000);
  drawSolveItem(1, "1", 10);
  drawSolveItem(2, "2", 35);
  drawSolveItem(3, "3", 60);
  drawSolveItem(4, "4", 85);
  drawSolveItem(5, "5", 110);
  drawSolveItem(6, "6", 135);
  drawSolveItem(7, "7", 160);
  drawSolveItem(8, "8", 185);
  drawSolveItem(9, "9", 210);
}
void drawSolveItem(int s_index, const char* text, int y) {
  if (solveIndex == s_index) {
    tft.setTextColor(ST77XX_YELLOW, 0x4000);
    tft.setCursor(250,y);tft.print("*");
  } else {
    tft.setTextColor(ST77XX_WHITE, 0x4000);
    tft.setCursor(250,y);tft.print(" ");
  }
  tft.setCursor(280, y);tft.print(text);
}

// =================================
// Werte anzeigen, bevor gelöst wird
// =================================
void display_values() {
  startzeit = millis();
  while (millis() - startzeit < 2000) {
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(250,20);
    tft.print("count");
    tft.setCursor(260,45);
    tft.print(count);
    tft.setCursor(250,70);
    tft.print("Maske");
    tft.setCursor(260,95);
    if (level == 1) {
      tft.print(ma+1);
    } else if (level == 2) {
      tft.print(mm+1);
    } else if (level == 3){
      tft.print(ms+1);
    }
    tft.setCursor(240,115);
    tft.print(levelstring);
    tft.setCursor(240,145);
    tft.print("(t ms:");
    tft.setCursor(250,170);
    tft.setTextSize(1);
    tft.print(gemessene_Zeit);
    tft.setTextSize(2);
    tft.setCursor(245,195);
    tft.print("eind.)");
  }; 
}
