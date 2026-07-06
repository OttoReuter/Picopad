# Sudoku für Picopad

Ein vollständig auf dem **Raspberry Pi Pico** laufendes Sudoku-Spiel für das **Picopad**, entwickelt mit der Arduino-IDE.

Das Projekt wurde bewusst als **monolithischer Arduino-Sketch mit modularer Architektur** umgesetzt. Obwohl es sich für die Arduino-IDE um einen einzigen Sketch handelt, sind die einzelnen Aufgaben klar voneinander getrennt. Dadurch bleibt der Code übersichtlich, leicht wartbar und einfach erweiterbar.

---

## Features

* Zufällig erzeugte Sudoku-Rätsel
* Garantiert eindeutige Lösung
* Vollständig integrierter Sudoku-Generator
* Integrierter Sudoku-Solver zur Validierung
* Komfortable Cursorsteuerung
* Direkte Eingabe der Zahlen
* Läuft vollständig ohne externe Daten oder Internetverbindung
* Optimiert für das Picopad

---

# Projektstruktur

Jede Datei besitzt eine klar definierte Aufgabe.

## Sudoku.ino

Die eigentliche Anwendung.

Verantwortlich für:

* Initialisierung
* Zustandsverwaltung
* Menü- und Spielsteuerung
* Hauptprogramm (`setup()` und `loop()`)

Hier befindet sich **keine Spiellogik**, **keine Darstellung** und **keine Sudoku-Erzeugung**.

---

## Display.cpp

Ausschließlich für die Darstellung zuständig.

Aufgaben:

* Zeichnen des Spielfeldes
* Cursordarstellung
* Hervorheben von Feldern
* Anzeigen von Menüs
* Bildschirmaktualisierung

Alle grafischen Funktionen sind hier gekapselt.

---

## Generator.cpp

Erzeugt neue Sudoku-Rätsel.

Verantwortlich für:

* Erzeugen vollständiger Sudoku-Lösungen
* Entfernen von Zahlen entsprechend dem Schwierigkeitsgrad
* Erzeugen spielbarer Rätsel

Die Datei enthält ausschließlich den Sudoku-Generator.

---

## Game.cpp

Die komplette Spiellogik.

Dazu gehören:

* Cursorbewegung
* Eingabe der Zahlen
* Löschen von Feldern
* Überprüfung erlaubter Eingaben
* Verwaltung des aktuellen Spielzustands

Alle Regeln des Spiels befinden sich ausschließlich hier.

---

## Solver.cpp

Sudoku-Solver und Validierung.

Aufgaben:

* Lösen eines Sudoku
* Prüfen der Gültigkeit
* Sicherstellen einer eindeutigen Lösung
* Unterstützung des Generators

Der Solver wird sowohl zur Validierung als auch zur Erzeugung neuer Rätsel verwendet.

---

# Architektur

```
              +----------------+
              |   Sudoku.ino   |
              | State / Loop   |
              +-------+--------+
                      |
      +---------------+----------------+
      |               |                |
      v               v                v
 Display.cpp     Game.cpp      Generator.cpp
 Rendering       Spielregeln     Rätsel erzeugen
                                      |
                                      v
                                Solver.cpp
                          Validierung / Lösung
```

Diese Aufteilung sorgt dafür, dass jede Datei genau eine Aufgabe besitzt (**Single Responsibility Principle**). Neue Funktionen lassen sich dadurch einfach ergänzen, ohne andere Bereiche des Programms zu beeinflussen.

---

# Bedienung

Die Steuerung erfolgt vollständig über die Tasten des Picopad.

| Taste   | Funktion                |
| ------- | ----------------------- |
| ↑ ↓ ← → | Cursor bewegen          |
| A       | Zahl erhöhen            |
| B       | Zahl verringern         |
| X       | Feld löschen (0 setzen) |
| Y       | Neues Sudoku erzeugen   |

*(Falls deine Tastenbelegung abweicht, diese Tabelle entsprechend anpassen.)*

---

# Technische Merkmale

* Arduino-IDE
* Raspberry Pi Pico (RP2040)
* Picopad
* Keine dynamische Speicherverwaltung
* Keine externen Bibliotheken für Sudoku
* Generator und Solver vollständig selbst implementiert

---

# Ziel des Projekts

Ziel war nicht nur die Entwicklung eines Sudoku-Spiels, sondern auch die Umsetzung einer klar strukturierten Softwarearchitektur innerhalb eines klassischen Arduino-Projekts.

Der Sketch demonstriert, wie sich auch größere Arduino-Anwendungen sauber modularisieren lassen, obwohl sie von der IDE als ein gemeinsamer Sketch verwaltet werden.

Dadurch bleibt der Code:

* übersichtlich
* wartbar
* leicht testbar
* einfach erweiterbar

---

## Lizenz

Dieses Projekt steht unter der im Repository angegebenen Lizenz.
