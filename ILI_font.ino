//Rotation test for 3.2" TFT LCD ILI9327
#include <Adafruit_GFX.h>    // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_TFTLCD.h> // https://github.com/adafruit/TFTLCD-Library

#define LCD_CS A3 // Chip Select
#define LCD_CD A2 // Command/Data
#define LCD_WR A1 // LCD Write
#define LCD_RD A0 // LCD Read
#define LCD_RESET A4

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TFT_WIDTH  320
#define TFT_HEIGHT 240

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


void drawColoredBars() {
  int w = tft.width();
  int h = tft.height();
  int barHeight = h / 6;

  tft.fillRect(0, 0, w, barHeight, RED);
  tft.fillRect(0, barHeight, w, barHeight, GREEN);
  tft.fillRect(0, barHeight * 2, w, barHeight, BLUE);
  tft.fillRect(0, barHeight * 3, w, barHeight, YELLOW);
  tft.fillRect(0, barHeight * 4, w, barHeight, CYAN);
  tft.fillRect(0, barHeight * 5, w, barHeight, MAGENTA);
  tft.setCursor(110, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("TFT Text Test");
}

void setup() {
	Serial.begin(115200);
	Serial.println("TFT Text test");

	uint16_t id = 0x9341;
	Serial.print("Trying ID: 0x");
	Serial.println(id, HEX);

	tft.reset();
	tft.begin(id);

	Serial.println("Drawing colored bars...");
	drawColoredBars();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any whitespace or newline characters

     
    }
  }
