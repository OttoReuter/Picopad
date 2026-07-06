#include "Game.h"
#include "Display.h"
#include "Generator.h"
#include "Solver.h"
#include "Masks_leicht.h"
#include "Masks_mittel.h"
#include "Masks_schwer.h"
#include "GameTypes.h"

extern Adafruit_ST7789 tft;
extern int array[81];
extern int baseArray[81];
extern GameState state;
bool solved = true;

// ==========================
// Position auswählen
// ==========================
void select_position() {
    tft.fillRect(240,5,75,230,0x4000);
    // Markierung setzen
    tft.drawRect(2, 2+j*26,234,26, ST77XX_YELLOW);
    tft.drawRect(2+i*26, 2, 26, 234, ST77XX_YELLOW);
    tft.drawRect(2+i*26, 2+j*26, 26, 26, ST77XX_RED);
    if (array[j * 9 + i] == 0) {
        tft.setCursor(250,20);
        tft.setTextColor(ST77XX_WHITE);
        tft.print("OK");
    }
    else
    {
        if (!solved) {
            tft.fillRect(240,5,70,30,0x4000);
        }
    }
}

// ==========================
// Zahl einsetzen
// ==========================
void select_number() {
    int zahl = solveIndex;

    tft.setCursor(2 + i * 26 + 8, 2 + j * 26 + 5);
    if (result[j * 9 + i] == zahl)     {
        tft.setTextColor(ST77XX_YELLOW);
        tft.print(zahl);
        array[j * 9 + i] = zahl;
        // prüfen ob fertig
        //bool solved = true;
        for (int k = 0; k < 81; k++)  {
            if (array[k] == 0) {
                solved = false;
                break;
            }
        }

        if (solved) {
            tft.setTextColor(ST77XX_WHITE);
            tft.setCursor(240,20);
            tft.print("fertig");
            tft.setCursor(245,70);
            tft.print("press");
            tft.setCursor(270,105);
            tft.print("B");
        }
    }
    else
    {
        // falsche zahl rot anzeigen und wieder löschen
        tft.setTextColor(ST77XX_RED);
        tft.print(zahl);
        delay(1000);
        tft.setTextColor(0x0320);
        tft.setCursor(2+i*26+ 8, 2+j*26+ 5);
        tft.print(zahl);
    }
}

// ==========================
// Sudoku starten
// ==========================
void start_sudoku() {
    bool ok = false;

    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(50,60);
    tft.print("Sudoku wird gebaut");
    tft.setCursor(70,120);
    tft.print("Bitte warten");
    while (!ok) {
        memcpy(array, baseArray, sizeof(array));
        array_generate();
        memcpy(result, array, sizeof(array));
        mask();

        ok = checkUnique(array);
    }

    mask_verified = true;
    gemessene_Zeit = millis();   // optional exakt wie vorher

    clear_screen();
    array_show();
    // Werte anzeigen oder weglassen
    //display_values();

    i = 0;
    j = 0;

    state = SELECT_POSITION;
    select_position();
}