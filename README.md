# Picopad

Es geht hier um Projekte mit dem Picopad des tschechischen Entwicklers Pajenicko (https://github.com/Pajenicko/Picopad). Ich habe den Bausatz bestellt und den RP2040 gegen einen RP2350-Plus von Waveshare ausgetauscht, da ich den Microcontroller von Pico 2 einsetzen wollte und den USB-C Anschluss bei dem o.g. Gerät bevorzuge. Wenn man nicht die vom Entwickler bereitgestellte SDK verwendet, kann man aus dem Datenblatt die Pinbelegung entnehmen, welche von anderen Herstellern leicht abweicht. Das ist aber kein Problem, denn die Anschlüsse der unterschiedlichen Pico-Modelle sind ja kompatitel.

![picopad_01](https://github.com/user-attachments/assets/24d1a4e2-037f-4912-931a-9e49ecea4a3e)

Hier gebe ich die Pinbelegung von Pajenicko an:

## Display:  
LCD_CS:  <b>GP21</b>,  LCD_RST: GP20,  LCD_MOSI: GP19,  LCD_SCK:  GP18,  LCD_DC:  GP17,  LCD_BL:  GP16,

## Tasten:
UP:  GP4,  DOWN:  GP5,  LEFT:  GP3,  RIGHT:  GP2,  X:  GP9,  Y:  GP8,  A:  GP7,  B:  GP6,

## Sound:
GP15,

## SD-Kartenslot:
SD_SPI1_SCK:  GP10,  SD_SPI1_MOSI: GP11,  SD_SPI1_MISO  GP12,   SD_SPI1_CS  GP 13,

