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

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

uint16_t commonIDs[] = {0x9327, 0x9341, 0x9486, 0x7783, 0x8357, 0x7575};
const int numIDs = sizeof(commonIDs) / sizeof(commonIDs[0]);

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
}

void setup() {
  Serial.begin(115200);
  Serial.println("TFT Controller Selector");
  Serial.println("Available IDs:");
  for (int i = 0; i < numIDs; i++) {
    Serial.print("  0x");
    Serial.println(commonIDs[i], HEX);
  }
  Serial.println("Type an ID (e.g., 9327) or 'reset' to reset the Arduino.");
  Serial.println();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any whitespace or newline characters

    if (input.equalsIgnoreCase("reset")) {
      Serial.println("Resetting Arduino...");
      delay(100);
      asm volatile ("jmp 0"); // Reset the Arduino
    } else {
      uint16_t id = (uint16_t)strtol(input.c_str(), NULL, 16); // Convert input to hex
      Serial.print("Trying ID: 0x");
      Serial.println(id, HEX);

      tft.reset();
      tft.begin(id);

      Serial.println("Drawing colored bars...");
      drawColoredBars();
      Serial.println("Done. Type another ID or 'reset' to reset.");
    }
  }
}