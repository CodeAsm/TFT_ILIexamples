//Rotation test for 3.2" TFT LCD ILI9327
#include <Adafruit_GFX.h>    // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_TFTLCD.h> // https://github.com/adafruit/TFTLCD-Library

#define LCD_CS A3 // Chip Select
#define LCD_CD A2 // Command/Data
#define LCD_WR A1 // LCD Write
#define LCD_RD A0 // LCD Read
#define LCD_RESET A4

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
int rotation = 0;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID(); // returns 0x9327 for ILI9327 LCD driver
  tft.begin(identifier);
  Serial.print("LCD driver = 0x");Serial.println(identifier, HEX);
}

void testLines() {
  tft.setRotation(rotation);
  tft.fillScreen(BLACK);
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();
  Serial.print("w="); Serial.print(w); Serial.print(", h="); Serial.println(h);

  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println("Rotation test");
  tft.print("Rotation = "); tft.println(rotation);

  //Draw a rectangle around the screen
  tft.drawRect(0, 0, w, h, BLUE);
  //Draw crossing lines
  tft.drawLine(0, 0, w, h, GREEN);
  tft.drawLine(0, w, h, 0, YELLOW);
  //Draw a circle at the screen center
  tft.fillCircle(w / 2, h / 2, 5, RED);

  //Draw a triangle at the screen center
  int cx = w / 2, cy = h / 2, i = 30;
  tft.drawTriangle(
    cx    , cy - i, // peak
    cx - i, cy + i, // bottom left
    cx + i, cy + i, // bottom right
    MAGENTA);
}

void loop(void) {
  for (rotation = 0; rotation <= 3; rotation++) {
    testLines();
    delay (10000);
  }
}