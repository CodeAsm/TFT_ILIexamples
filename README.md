# ILI9327 Rotation Test

This project demonstrates a few things for TFT LCD screen using the ILI series of drivers. The test includes drawing shapes, lines, and text on the screen while rotating the display. and detecting or selecting the controler.

Note, this project skips using the arduino IDE and we compile using my silly makefile.

- ILI_idtester.ino allows one to type an display controller id and see if it works.
- ILI_font.ino  draws some text as a test, then prints what it recieves over serial.

## Features

- Displays text and shapes on the ILI-based TFT LCD.
- Uses the Adafruit GFX and Adafruit TFTLCD libraries for graphics rendering.

## How to Compile

To compile and upload the code to your Arduino, follow these steps:

### Prerequisites

1. Install the following libraries:
	- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
	- [Adafruit TFTLCD Library](https://github.com/adafruit/TFTLCD-Library)
	- [Adafruit BusIO Library](https://github.com/adafruit/Adafruit_BusIO)
2. Ensure `avr-gcc` and `avrdude` are installed on your system.

### Compilation Steps

1. Clone the repository or copy the source files to your working directory.
2. Use the provided `Makefile` to compile the project:
	```bash
	make clean
	make
	make upload
	```
3. The Makefile automates the following steps:
	- **Preprocessing the .ino File**: Converts the `.ino` file into `.cpp` format and adds necessary Arduino headers.
	- **Compiling Arduino Core Files**: Builds the `core.a` archive locally, including essential files like `wiring.c`, `wiring_digital.c`, and `HardwareSerial.cpp`.
	- **Compiling Adafruit Libraries**: Compiles `Adafruit_GFX`, `Adafruit_TFTLCD`, and `Adafruit_BusIO` libraries.
	- **Linking Object Files**: Links the compiled object files, libraries, and core files to generate the final `.hex` file.
	- **Optimizing Code Size**: Uses `-flto` (link-time optimization) to reduce the size of the compiled binary.
	- **Uploading the Code**: Uploads the `.hex` file to the Arduino using `avrdude`.

### Notes

- Ensure the correct board and port are specified in the `Makefile`.
- If you encounter memory overflow issues, consider removing unused features or optimizing the code further.
- Why are we not using the Arduino IDE? Because we dont want to, there is no why else. 

## License

This project is licensed under the MIT License.

