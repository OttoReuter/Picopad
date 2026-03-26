#ifndef SPRITE_H
#define SPRITE_H

#include <Arduino.h>

// Sprite-Größe
constexpr int SPRITE_WIDTH  = 36;   // 32
constexpr int SPRITE_HEIGHT = 36;   // 32
//constexpr int SPRITE_PADDING = 0; // extra Pixel rundherum für sauberes restore

struct Sprite {
    int16_t x;
    int16_t y;
    // Hintergrundbuffer mit Puffer
    //uint16_t bg[(SPRITE_WIDTH + 2 * SPRITE_PADDING) * (SPRITE_HEIGHT + 2 * SPRITE_PADDING)];
    uint16_t bg[SPRITE_WIDTH * SPRITE_HEIGHT];
};

#endif