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
extern int points;
extern int failed;
extern bool isready;
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
        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(250,20);
        tft.print("OK");
        show_points();
    }
    else
    {
        if (!solved) {
            tft.fillRect(240,5,70,30,0x4000);
            show_points();
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
        points += 15;
        array[j * 9 + i] = zahl;
        // prüfen ob fertig
        for (int k = 0; k < 81; k++)  {
            if (array[k] == 0) {
                solved = false;
                isready = false;
                break;
            } else {
                isready = true;
            }
        }
    }
    else
    {
        // falsche Zahl rot anzeigen und wieder löschen
        tft.setTextColor(ST77XX_RED);
        tft.print(zahl);
        points -= 15;
        failed += 1;        
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
    tft.setCursor(70,90);
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

    //display_values();

    i = 0;
    j = 0;

    state = SELECT_POSITION;
    select_position();
}
