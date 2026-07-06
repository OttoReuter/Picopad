#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

#include "GameTypes.h"

// externe Daten
extern int array[81];
extern int result[81];

extern int i;
extern int j;

extern int solveIndex;
extern int level;
extern String levelstring;

extern bool mask_verified;
extern unsigned long gemessene_Zeit;

// Funktionen
void start_sudoku();
void select_position();
void select_number();

#endif