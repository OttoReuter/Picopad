#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Fonts/FreeSansBold12pt7b.h>

// ==== Pins ====
#define TFT_CS   21
#define TFT_RST  20
#define TFT_DC   17
#define TFT_BLK  16

#define KEY_DOWN    5
#define KEY_LEFT    3
#define KEY_UP      4
#define KEY_RIGHT   2
#define KEY_CENTER  9
#define KEY_A       7
#define KEY_B       6

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// ==== Mandelbrot & Zoom ====
const int MAX_ITER = 300;

// ====================== Smooth Palette ======================
struct RGB {
  uint8_t r, g, b;
};

#define PALETTE_SIZE 7
RGB PALETTE_RGB[PALETTE_SIZE] = {
  {0,   0,   0},     // schwarz
  {0,   0, 255},     // blau
  {0, 255, 255},     // cyan
  {255,  0,   0},    // rot
  {255,  0, 255},     // magenta
  {0, 255,   0},     // grün
  {255,255,   0}    // gelb
};

uint16_t getSmoothColor(float v) {
  int i1 = (int)v % PALETTE_SIZE;
  int i2 = (i1 + 1) % PALETTE_SIZE;
  float t = v - (int)v;

  uint8_t r = PALETTE_RGB[i1].r + t * (PALETTE_RGB[i2].r - PALETTE_RGB[i1].r);
  uint8_t g = PALETTE_RGB[i1].g + t * (PALETTE_RGB[i2].g - PALETTE_RGB[i1].g);
  uint8_t b = PALETTE_RGB[i1].b + t * (PALETTE_RGB[i2].b - PALETTE_RGB[i1].b);

  return tft.color565(r, g, b);
}

// Mandelbrot-Koordinaten
float xmin = -1.8, xmax = 0.6;
//float xmin = -1.5, xmax = 0.5;
float ymin = -1.0, ymax = 1.0;

// Zoom-Faktor:
// 0.5   → Rechteck zeigt 1/4 des Fraktalbereichs
// 0.25  → Rechteck zeigt 1/16 des Fraktalbereichs (wie aktuell)
// 0.125 → Rechteck zeigt 1/64 des Fraktalbereichs
float zoomFactor = 0.125;

// ==== Rechteck für Zoom (immer 1/16 des Displays) ====
#define RECT_W 35
#define RECT_H 30
#define RECT_HALF_W (RECT_W / 2)
#define RECT_HALF_H (RECT_H / 2)
#define RECT_PIXELS (RECT_W*2 + RECT_H*2)
uint16_t rectBackground[RECT_PIXELS];

// ==== Mittelpunkt des Rechtecks ====
int cursorX, cursorY;

// Flag, ob Fraktal fertig ist
bool fractalReady = false;

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(TFT_BLK, OUTPUT);
  analogWrite(TFT_BLK, 255);
  tft.init(240, 320);
  tft.setRotation(3); 
  tft.fillScreen(ST77XX_BLUE);

  // Pins als Input
  pinMode(KEY_RIGHT, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_CENTER, INPUT_PULLUP);
  pinMode(KEY_A, INPUT_PULLUP);
  pinMode(KEY_B, INPUT_PULLUP);

  //// Palette
  //PALETTE[0] = tft.color565(0, 0, 0);
  //PALETTE[1] = tft.color565(0, 0, 255);
  //PALETTE[2] = tft.color565(0, 255, 255);
  //PALETTE[3] = tft.color565(0, 255, 0);
  //PALETTE[4] = tft.color565(255, 255, 0);
  //PALETTE[5] = tft.color565(255, 0, 0);
  //PALETTE[6] = tft.color565(255, 0, 255);

  // Rechteck in Bildschirmmitte
  cursorX = tft.width() / 2;
  cursorY = tft.height() / 2;

  // Initiales Fraktal zeichnen
  drawMandelbrot();
  fractalReady = true;

  // Rechteck speichern und zeichnen
  saveRectBackground(cursorX, cursorY);
  drawZoomRect();
}

// ================= LOOP =================
void loop() {
  handleInput(); // Rechteck bewegen & Zoom/Reset
}

// ================= EINGABEVERARBEITUNG =================
void handleInput() {
  int step = 5; // Anzahl Pixel pro Tastendruck
  int dx = 0, dy = 0;
  if (digitalRead(KEY_UP) == LOW)    dy = -step;
  if (digitalRead(KEY_DOWN) == LOW)  dy = step;
  if (digitalRead(KEY_LEFT) == LOW)  dx = -step;
  if (digitalRead(KEY_RIGHT) == LOW) dx = step;

  delay(100); // kurze Verzögerung

  int newX = constrain(cursorX + dx, RECT_HALF_W, tft.width() - RECT_HALF_W);
  int newY = constrain(cursorY + dy, RECT_HALF_H, tft.height() - RECT_HALF_H);

  if (newX != cursorX || newY != cursorY) {
    restoreRectBackground(cursorX, cursorY);
    cursorX = newX;
    cursorY = newY;
    saveRectBackground(cursorX, cursorY);
    drawZoomRect();
  }

  // Taste A → hineinzoomen
  if (digitalRead(KEY_A) == LOW) {
    restoreRectBackground(cursorX, cursorY);
    zoomOnCursor(true);
    drawMandelbrot();
    saveRectBackground(cursorX, cursorY);
    drawZoomRect();
    delay(200);
  }

  // Taste B → zurücksetzen
  if (digitalRead(KEY_B) == LOW) {
    restoreRectBackground(cursorX, cursorY);
    tft.fillScreen(ST77XX_BLUE);
    xmin = -1.5; xmax = 0.5;
    ymin = -1.0; ymax = 1.0;
    drawMandelbrot();
    cursorX = tft.width() / 2;
    cursorY = tft.height() / 2;
    saveRectBackground(cursorX, cursorY);
    drawZoomRect();
    delay(200);
  }

  // Taste CENTER → Werte anzeigen
  if (digitalRead(KEY_CENTER) == LOW) {
    tft.fillScreen(ST77XX_BLUE);
    showCoordinates();
    //delay(500); // Kurze Pause, damit die Anzeige sichtbar bleibt
  }
}

void showCoordinates() {
  //tft.setTextColor(ST77XX_WHITE, ST77XX_BLUE); // Weiß auf blauem Hintergrund
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.setCursor(15, 10); // Oben links

  //tft.fillRect(0, 0, tft.width(), 60, ST77XX_BLUE); // Hintergrund für Text

  tft.print(" xmin: \n "); tft.print(xmin, 8);tft.print("\n ");
  tft.print(" xmax: \n "); tft.println(xmax, 8);tft.print("\n ");
  tft.print(" ymin: \n "); tft.print(ymin, 8);tft.print("\n ");
  tft.print(" ymax: \n "); tft.println(ymax, 8);
}

// ================= MANDELBROT =================
void drawMandelbrot() {
  for (int y = 0; y < tft.height(); y++) {
    float ci = ymin + (ymax - ymin) * y / tft.height();
    for (int x = 0; x < tft.width(); x++) {
      float cr = xmin + (xmax - xmin) * x / tft.width();
      float zr = 0.0, zi = 0.0;
      int iter = 0;

      while (zr*zr + zi*zi <= 4.0 && iter < MAX_ITER) {
        float tmp = zr*zr - zi*zi + cr;
        zi = 2.0*zr*zi + ci;
        zr = tmp;
        iter++;
      }

      uint16_t color;
      if (iter == MAX_ITER) {
        color = tft.color565(0, 0, 0);
      } else {
        float mag2 = zr*zr + zi*zi;
        float smooth = iter + 1 - log(log(mag2)/2.0f)/log(2.0f);
        smooth *= 0.15f; // Farbübergang-Geschwindigkeit
        color = getSmoothColor(smooth);
      }

      tft.drawPixel(x, y, color);
    }
  }
}

// ================= ZOOM =================
void zoomOnCursor(bool zoomIn) {
  // 1️⃣ Mittelpunkt des Rechtecks in Pixeln
  int centerX = cursorX;
  int centerY = cursorY;

  // 2️⃣ Fraktal-Koordinaten des Rechteckmittelpunkts
  float cX0 = xmin + (xmax - xmin) * centerX / tft.width();
  float cY0 = ymin + (ymax - ymin) * centerY / tft.height();

  // 3️⃣ Neue Breite/Höhe des Ausschnitts nach Zoom
  // Je kleiner zoomFactor, desto stärker gezoomt (kleinerer Ausschnitt)
  float xRange = (xmax - xmin) * (zoomIn ? zoomFactor : (1.0 / zoomFactor));
  float yRange = (ymax - ymin) * (zoomIn ? zoomFactor : (1.0 / zoomFactor));

  // 4️⃣ xmin/ymin so setzen, dass der Fraktal-Mittelpunkt unter dem Rechteck bleibt
  xmin = cX0 - xRange * ((float)centerX / tft.width());
  ymin = cY0 - yRange * ((float)centerY / tft.height());
  xmax = xmin + xRange;
  ymax = ymin + yRange;

  // 5️⃣ Rechteckmittelpunkt neu berechnen (falls durch Zoom verschoben)
  cursorX = (cX0 - xmin) / (xmax - xmin) * tft.width();
  cursorY = (cY0 - ymin) / (ymax - ymin) * tft.height();

  // 6️⃣ Sicherheitsbegrenzung: Mittelpunkt innerhalb des Displays halten
  cursorX = constrain(cursorX, 0, tft.width() - 1);
  cursorY = constrain(cursorY, 0, tft.height() - 1);
}

// ================= RECHTECK =================
void drawZoomRect() {
  tft.drawRect(cursorX - RECT_HALF_W, cursorY - RECT_HALF_H, RECT_W, RECT_H, ST77XX_WHITE);
}

void saveRectBackground(int cx, int cy) {
  int x0 = cx - RECT_HALF_W;
  int y0 = cy - RECT_HALF_H;
  int idx = 0;

  // obere Kante
  for (int x = 0; x < RECT_W; x++)
    rectBackground[idx++] = getPixelColor(x0 + x, y0);
  // untere Kante
  for (int x = 0; x < RECT_W; x++)
    rectBackground[idx++] = getPixelColor(x0 + x, y0 + RECT_H - 1);
  // linke Kante
  for (int y = 0; y < RECT_H; y++)
    rectBackground[idx++] = getPixelColor(x0, y0 + y);
  // rechte Kante
  for (int y = 0; y < RECT_H; y++)
    rectBackground[idx++] = getPixelColor(x0 + RECT_W - 1, y0 + y);
}

void restoreRectBackground(int cx, int cy) {
  int x0 = cx - RECT_HALF_W;
  int y0 = cy - RECT_HALF_H;
  int idx = 0;

  for (int x = 0; x < RECT_W; x++) tft.drawPixel(x0 + x, y0, rectBackground[idx++]);
  for (int x = 0; x < RECT_W; x++) tft.drawPixel(x0 + x, y0 + RECT_H - 1, rectBackground[idx++]);
  for (int y = 0; y < RECT_H; y++) tft.drawPixel(x0, y0 + y, rectBackground[idx++]);
  for (int y = 0; y < RECT_H; y++) tft.drawPixel(x0 + RECT_W - 1, y0 + y, rectBackground[idx++]);
}

// ================= PIXELFARBE =================
uint16_t getPixelColor(int px, int py) {
  if (px < 0 || px >= tft.width() || py < 0 || py >= tft.height()) {
    return tft.color565(0,0,0);
  }
  float ci = ymin + (ymax - ymin) * py / tft.height();
  float cr = xmin + (xmax - xmin) * px / tft.width();
  float zr = 0.0, zi = 0.0;
  int iter = 0;

  while (zr*zr + zi*zi <= 4.0 && iter < MAX_ITER) {
    float tmp = zr*zr - zi*zi + cr;
    zi = 2.0*zr*zi + ci;
    zr = tmp;
    iter++;
  }

  if (iter == MAX_ITER) return tft.color565(0,0,0);

  float mag2 = zr*zr + zi*zi;
  float smooth = iter + 1 - log(log(mag2)/2.0f)/log(2.0f);
  smooth *= 0.15f;
  return getSmoothColor(smooth);
}
