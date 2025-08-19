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
int currentRotation = 0; // Default rotation

void SetupScreen() {
  int w = tft.width();
  int h = tft.height();
  int barHeight = h / 6;

  tft.fillRect(0, 0, w, barHeight, RED);
  tft.fillRect(0, barHeight, w, barHeight, GREEN);
  tft.fillRect(0, barHeight * 2, w, barHeight, BLUE);
  tft.fillRect(0, barHeight * 3, w, barHeight, YELLOW);
  tft.fillRect(0, barHeight * 4, w, barHeight, CYAN);
  tft.fillRect(0, barHeight * 5, w, barHeight, MAGENTA);

  tft.setCursor(w / 2 - 50, barHeight / 2); // Center the text
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("TFT Text Test");
}

void setup() {
  Serial.begin(115200);
  Serial.println("TFT Controller Selector with Rotation");

  Serial.print("Display width: ");
  Serial.println(tft.width());
  Serial.print("Display height: ");
  Serial.println(tft.height());
  
  Serial.println("Available IDs:");
  for (int i = 0; i < numIDs; i++) {
    Serial.print("  0x");
    Serial.println(commonIDs[i], HEX);
  }
  Serial.println("Type an ID (e.g., 9327), 'rotation' to change rotation, or 'reset' to reset the Arduino.");
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
    } else if (input.equalsIgnoreCase("rotation")) {
      currentRotation = (currentRotation + 1) % 4; // Cycle through 0, 1, 2, 3
      tft.setRotation(currentRotation);
      Serial.print("Rotation set to: ");
      Serial.println(currentRotation);
	  
	tft.invertDisplay(true); // Invert colors for better visibility

      SetupScreen(); // Redraw the bars with the new rotation
    } else {
      uint16_t id = (uint16_t)strtol(input.c_str(), NULL, 16); // Convert input to hex
      Serial.print("Trying ID: 0x");
      Serial.println(id, HEX);

      tft.reset();
      tft.begin(id);

	tft.invertDisplay(true); // Invert colors for better visibility


      Serial.println("Drawing colored bars...");
      SetupScreen();
      Serial.println("Done. Type another ID, 'rotation', or 'reset' to reset.");
    }
  }
}