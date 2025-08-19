//Rotation test for 3.2" TFT LCD ILI9327
#include <Adafruit_GFX.h>    // https://github.com/adafruit/Adafruit-GFX-Library
#include <CodeAsm_TFTLCD.h> // https://github.com/CodeAsm/TFTLCD-Library
#include <registers.h> // For register definitions

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

//#define TFT_WIDTH  320	//320
//#define TFT_HEIGHT 240	//240

//#define TFTWIDTH 410
//#define TFTHEIGHT 320

CodeAsm_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


void SetupScreen() {
  int w = tft.width();
  int h = tft.height();
  int barHeight = h / 8;
  int barWidth = w / 8;
//
//	---> x
//  |
//	|
//	V y
//

		//	   x, y, w, h, color
  tft.fillRect(0, 0, barWidth, h, RED);
  tft.fillRect(barWidth, 0, barWidth, h, GREEN);
  tft.fillRect(barWidth * 2, 0, barWidth, h, BLUE);
  tft.fillRect(barWidth * 3, 0, barWidth, h, YELLOW);
  tft.fillRect(barWidth * 4, 0, barWidth, h, CYAN);
  tft.fillRect(barWidth * 5, 0, barWidth, h, MAGENTA);
  tft.fillRect(barWidth * 6, 0, barWidth, h, BLACK);
  tft.fillRect(barWidth * 7, 0, barWidth, h, WHITE);
  tft.setCursor(2, 2);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Font test 1.3");
  tft.print("With: ");tft.println(tft.width());
  tft.print("Height: ");tft.println(tft.height());
  

}

void setup() {
	Serial.begin(115200);
	Serial.println("TFT Text test");

	uint16_t id = 0x9341; // 0x8357 Default ID for testing, can be changed to any common ID
	Serial.print("Trying ID: 0x");
	Serial.println(id, HEX);

	tft.reset();
	tft.begin(id);
	tft.setRotation(1); // Set rotation to 1 (portrait mode)
  // Send a custom MADCTL command to enable mirroring
  // ILI9341_MADCTL_MV 	
  // ILI9341_MADCTL_MX
  // ILI9341_MADCTL_MY
  // ILI9341_MADCTL_BGR
	// Set the memory access control register to enable mirroring
 	//tft.writeRegister8(ILI9341_MADCTL, ILI9341_MADCTL_BGR);

	//Latest for 9341
    tft.writeRegister8(ILI9341_MADCTL, ILI9341_MADCTL_MX | ILI9341_MADCTL_MV | ILI9341_MADCTL_ML); // MADCTL
	Serial.println("Drawing colored bars...");
	SetupScreen();
	
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any whitespace or newline characters

     
    }
  }
