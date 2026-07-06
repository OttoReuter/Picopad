#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Displayobjekt kommt aus Sudoku.ino
extern Adafruit_ST7789 tft;

// Globale Daten
extern int i;
extern int j;
extern unsigned long startzeit;

extern int array[81];
extern int x_pos[9];
extern int y_pos[9];

extern int menuIndex;
extern int solveIndex;

extern int level;
extern int ma;
extern int mm;
extern int ms;
extern int count;
extern unsigned long gemessene_Zeit;
extern String levelstring;

// Funktionen
void clear_screen();
void draw_gitter();
void array_show();

void drawMenu();
void drawMenuItem(int index, const char* text, int y);

void drawSideboard();
void drawSolveItem(int index, const char* text, int y);

void display_values();

#endif
