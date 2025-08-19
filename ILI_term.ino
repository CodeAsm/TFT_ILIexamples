//Terminal test for 3.2" TFT LCD ILI9327
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
//
//	---> x
//  |
//	|
//	V y
//

		//	   x, y, w, h, color
  tft.fillRect(0, 0, w, h, BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("Terminal 0.1");
  tft.print(" With: ");tft.print(tft.width());
  tft.print(" Height: ");tft.println(tft.height());
  

}

void setup() {
	Serial.begin(115200);
	Serial.println("TFT Terminal");

	uint16_t id = 0x9341; // 0x8357 Default ID for testing, can be changed to any common ID

	tft.reset();
	tft.begin(id);
	tft.setRotation(1); // Set rotation to 1 (portrait mode)
	tft.setScrollMargins(0,100); // Set scroll margins to 0
    tft.writeRegister8(ILI9341_MADCTL, ILI9341_MADCTL_MX | ILI9341_MADCTL_MV | ILI9341_MADCTL_ML); // MADCTL
	Serial.println("...");
	SetupScreen();
	
}
void printdebug(int cursorX,int cursorY) {
	int x =	tft.getCursorX();
	int y = tft.getCursorY();
	if (x != cursorX || y != cursorY) {
	
		tft.setCursor(220, 0); // Set cursor position for debug info
		tft.fillRect(220, 0, 150, 10, BLACK); // Clear previous debug info block
		tft.print("Cursor X: ");
		tft.print(x);
		tft.print(" Y: ");
		tft.print(y);
		tft.setCursor(x, y); // Restore cursor position
	}
}

void loop() {
	static int cursorX = 0;
	
					tft.scrollTo(0, 100); // Scroll up by 10 pixels
	static int cursorY = 0;
	if (tft.getCursorY() + 10 >= 50){ //tft.height()) {
					// If the cursor is at the bottom of the screen, scroll up
					tft.scrollTo(0, 100); // Scroll up by 10 pixels
					tft.setCursor(0,10);
				}
	printdebug(cursorX,cursorY);
	cursorX = tft.getCursorX();
	cursorY = tft.getCursorY();
	
	if (Serial.available() > 0) {
		char received = Serial.read(); // Read the incoming byte
		switch (received) {
			case '\r': // Carriage return
			case '\n': // Newline
				// Move cursor to the beginning of the current line
				//tft.setCursor(2, tft.getCursorY());
				// Move cursor to the next line
				tft.setCursor(0, tft.getCursorY() + 10); // Adjust line height as needed
				break;
			case 8: // ASCII value for backspace is 8
				// Move cursor back and overwrite with a space
				tft.setCursor(tft.getCursorX() - 6, tft.getCursorY()); // Adjust character width as needed
				tft.print(' ');
				tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
				break;
			case 27: // Escape character
				// Handle escape sequences if needed
				// For example, you can clear the screen or perform other actions
				tft.fillRect(0, 0, tft.width(), tft.height(), BLACK); // Clear the screen
				SetupScreen(); // Reinitialize the screen
				break;
			default:
				// Handle other characters normally
				tft.print(received);
				break;
		} 
	
		if (Serial.availableForWrite() > 0) {
			// If there is space to write, you can send data back
			// For example, echo the received character back
			if (received == '\r' || received == '\n') {
				Serial.write('\r');
				Serial.write('\n');
			} else {
				Serial.write(received); // Uncomment if you want to echo back
			}
			// delay(10); // Adjust the delay as needed
		} else {
			// If no data is available
			//delay(100); // Adjust the delay as needed
		}
	}
}

