#include "../engine/SpriteDraw.h"
#include <Arduino.h>
#include <Adafruit_ST7789.h>
#include <avr/pgmspace.h>  // falls pgm_read_word nötig

extern Adafruit_ST7789 tft;  // dein TFT aus main.ino

void drawSpriteTransparentXY(int x, int y, const uint16_t *bitmap, uint16_t transparentColor) {
    for(int j=0;j<32;j++){
        for(int i=0;i<32;i++){
            uint16_t color = pgm_read_word(&bitmap[j*32+i]);
            if(color != transparentColor) tft.drawPixel(x+i, y+j, color);
        }
    }
}