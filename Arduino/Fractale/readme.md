# Fractalbilder auf dem Picopad erzeugen

Ich zeige, wie man mit dem Picopad in der Arduino-IDE die Mandelbrotmenge darstellt. Es handelt sich um die Darstellung
der s.g. Apfelmännchen. Der Mikrocontroller RP2350, welcher bei mir verbaut ist, zeigt hier gegenüber dem RP2040 eine noch
beeindruckendere Rechenleistung. Bei gleichem Quellcode ist er mehr als doppelt so schnell mit dem Aufbau der Grafik fertig.

## Hier sind ein paar Hinweise zur Bedienung des Programms:

1. Beim Einschalten wird die komplette Teilmenge dargestellt und nach Fertigstellung ein Rechteck für einen Zoom eingeblendet.

2. Mit den Cursor-Tasten navigieren Sie zu einer beliebigen Stelle innerhalb des Displays und bestätigen mit der A-Taste.So legen Sie den Zoombereich fest. Darauf wird dieser Ausschnitt berechnet und dargestellt.

3. (optional) Mit einem Klick auf 'X' werden die augenblicklichen Werte angezeigt. Danach wird bei A-Taste die Berechnung fortgeführt, wo das Recheck zuletzt stand bzw. bei B-Taste 4. ausgeführt.

4. Ein Klick auf die B-Taste setzt die Grafik auf die Anfangswerte zurück. 
