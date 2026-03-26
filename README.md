# Picopad

Es geht hier um Projekte mit dem Picopad des tschechischen Entwicklers Pajenicko (https://github.com/Pajenicko/Picopad). Ich habe den Bausatz bestellt und den RP2040 gegen einen RP2350-Plus von Waveshare ausgetauscht, da ich den Microcontroller von Pico 2 einsetzen wollte und den USB-C Anschluss bei dem o.g. Gerät bevorzuge. Wenn man nicht die vom Entwickler bereitgestellte SDK verwendet, kann man aus dem Datenblatt die Pinbelegung entnehmen, welche von anderen Herstellern leicht abweicht. Das ist aber kein Problem, denn die Anschlüsse der unterschiedlichen Pico-Modelle sind ja kompatitel.

![picopad_01](https://github.com/user-attachments/assets/24d1a4e2-037f-4912-931a-9e49ecea4a3e)

Hier gebe ich die Pinbelegung von Pajenicko an:

Display:  
          LCD_CS   GP 21,
          
          LCD_RST  GP 20,
          LCD_MOSI GP 19,
          LCD_SCK  GP 18,
          LCD_DC   GP 17,
          LCD_BL   GP 16,

          
