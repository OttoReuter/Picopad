#include "Generator.h"
#include "Masks_leicht.h"
#include "Masks_mittel.h"
#include "Masks_schwer.h"

// wir brauchen Zugriff auf globale Variablen
extern int i;
extern int level;
extern int ma, mm, ms;
extern int count;

// ==================
// Zahlen vertauschen
// ==================
void swapNumbers() {
  int zahl1 = random(1, 10);
  int zahl2;

  do {
    zahl2 = random(1, 10);
  } while (zahl1 == zahl2);

  for (i = 0; i < 81; i++) {
    if (array[i] == zahl1)
      array[i] = zahl2;
    else if (array[i] == zahl2)
      array[i] = zahl1;
  }
}

// =============================
// zwei Zeilen im Block tauschen
// =============================
void swapRows() {
  int block = random(3);
  int r1 = block * 3 + random(3);
  int r2;

  do {
    r2 = block * 3 + random(3);
  } while (r1 == r2);

  for (int c = 0; c < 9; c++) {
    int t = array[r1 * 9 + c];
    array[r1 * 9 + c] = array[r2 * 9 + c];
    array[r2 * 9 + c] = t;
  }
}

// =============================
// zwei Spalten im Block tauschen
// =============================
void swapCols() {
  int block = random(3);
  int c1 = block * 3 + random(3);
  int c2;

  do {
    c2 = block * 3 + random(3);
  } while (c1 == c2);

  for (int r = 0; r < 9; r++) {
    int t = array[r * 9 + c1];
    array[r * 9 + c1] = array[r * 9 + c2];
    array[r * 9 + c2] = t;
  }
}

// =============================
// Zeilenblöcke tauschen
// =============================
void swapRowBlocks() {
  int b1 = random(3);
  int b2;

  do {
    b2 = random(3);
  } while (b1 == b2);

  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 9; c++) {
      int i1 = (b1 * 3 + r) * 9 + c;
      int i2 = (b2 * 3 + r) * 9 + c;

      int t = array[i1];
      array[i1] = array[i2];
      array[i2] = t;
    }
  }
}

// =============================
// Spaltenblöcke tauschen
// =============================
void swapColBlocks() {
  int b1 = random(3);
  int b2;

  do {
    b2 = random(3);
  } while (b1 == b2);

  for (int c = 0; c < 3; c++) {
    for (int r = 0; r < 9; r++) {
      int i1 = r * 9 + b1 * 3 + c;
      int i2 = r * 9 + b2 * 3 + c;

      int t = array[i1];
      array[i1] = array[i2];
      array[i2] = t;
    }
  }
}

// ========================
// Matrix transponieren
// ========================
void transpose() {
  for (int r = 0; r < 9; r++) {
    for (int c = r + 1; c < 9; c++) {
      int t = array[r * 9 + c];
      array[r * 9 + c] = array[c * 9 + r];
      array[c * 9 + r] = t;
    }
  }
}

// ========================
// Sudoku generieren
// ========================
void array_generate() {
  //int count = random(15, 31);
  count = random(15, 31);

  for (int k = 0; k < count; k++) {
    switch (random(6)) {
      case 0: swapNumbers(); break;
      case 1: swapRows(); break;
      case 2: swapCols(); break;
      case 3: swapRowBlocks(); break;
      case 4: swapColBlocks(); break;
      case 5: transpose(); break;
    }
  }
}

// ========================
// Maske anwenden
// ========================
void mask() {

  int level_local = level;

  if (level_local == 1) {
    ma = random(10);
    for (int k = 0; k < 81; k++) {
      if (masks_a[ma][k] == 0)
        array[k] = 0;
    }
  }

  else if (level_local == 2) {
    mm = random(10);
    for (int k = 0; k < 81; k++) {
      if (masks_m[mm][k] == 0)
        array[k] = 0;
    }
  }

  else if (level_local == 3) {
    ms = random(10);
    for (int k = 0; k < 81; k++) {
      if (masks_s[ms][k] == 0)
        array[k] = 0;
    }
  }
}