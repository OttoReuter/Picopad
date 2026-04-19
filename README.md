# Picopad

Sie finden hier um Projekte mit dem Picopad des tschechischen Entwicklers Pajenicko (https://github.com/Pajenicko/Picopad). Ich habe für mich einen Bausatz (umgerechnet rund 38 Euro) bestellt. Die Lieferung erfolgte schnell. Alles war gut sortiert verpackt und vollständig. Für die Lötarbeit gibt es von Pajenicko eine Montageanleitung im Netz mit Video. Dort werden alle Schritte übersichtlich gezeigt. Ich habe den mitgelieferten RP2040 Pico W gegen einen RP2350-Plus von Waveshare ausgetauscht, da ich den Microcontroller vom Pico 2 einsetzen wollte und den USB-C Anschluss bei dem o.g. Gerät bevorzuge. Wenn man nicht die vom Entwickler bereitgestellte SDK verwendet, muss man aus dem Datenblatt des Picopad die Pinbelegung entnehmen, welche von anderen Herstellern leicht abweicht. Das ist aber kein Problem, denn die Anschlüsse der unterschiedlichen Pico-Modelle sind ja kompatitel.

![picopad_01](https://github.com/user-attachments/assets/24d1a4e2-037f-4912-931a-9e49ecea4a3e)

Hier gebe ich die Pinbelegung für das Picopad von Pajenicko an:

## Display:  
LCD_CS:  <b>GP21</b>,  LCD_RST: <b>GP20</b>,  LCD_MOSI: <b>GP19</b>,  LCD_SCK:  <b>GP18</b>,  LCD_DC:  <b>GP17</b>,  LCD_BL:  <b>GP16</b>,

## Tasten:
UP:  <b>GP4</b>,  DOWN:  <b>GP5</b>,  LEFT:  <b>GP3</b>,  RIGHT:  <b>GP2</b>,  X:  <b>GP9</b>,  Y:  <b>GP8</b>,  A:  <b>GP7</b>,  B:  <b>GP6</b>,

## Sound:
<b>GP15</b>,

## SD-Kartenslot:
SD_SPI1_SCK:  <b>GP10</b>,  SD_SPI1_MOSI: <b>GP11</b>,  SD_SPI1_MISO  <b>GP12</b>,   SD_SPI1_CS  <b>GP13</b>,

Die Belegung des externen Anschlusses für die Nutzung von Sensoren gibt Pajenicko auf seiner Seite (siehe oben) an.
## Externer Anschluss:
<b>GP0, GP1, GP14, GP26, GP27, GP28, ADC_VREF, 3V3, VBAT</b> und <b>GND. </b> 

Damit können Sie das Picopad unabhängig von der mitgelieferten SDK in der Arduino-IDE oder CircuitPython bzw. MicroPython, je nach Ihrer Vorliebe programmieren. Entsprechende Beispiele werde ich hier auf Github veröffentlichen.
