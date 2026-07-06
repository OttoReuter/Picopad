#ifndef GENERATOR_H
#define GENERATOR_H

#include <Arduino.h>

// arbeitet auf dem Sudoku-Array
extern int array[81];

// Generator-Funktionen
void array_generate();
void mask();

void swapNumbers();
void swapRows();
void swapCols();
void swapRowBlocks();
void swapColBlocks();
void transpose();

#endif