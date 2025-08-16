# Define arduino ide SDK version
ARDUINO_SDK_VERSION = 1.8.6

# Define paths
#ARDUINO_LIBS = $(HOME)/.arduino15/packages/arduino/hardware/avr/$(ARDUINO_SDK_VERSION)/libraries
ARDUINO_LIBS = $(HOME)/.arduino15/libraries
ARDUINO_CORE = $(HOME)/.arduino15/packages/arduino/hardware/avr/$(ARDUINO_SDK_VERSION)/cores/arduino
ARDUINO_VARIANTS = $(HOME)/.arduino15/packages/arduino/hardware/avr/$(ARDUINO_SDK_VERSION)/variants
ARDUINO_LIB = $(HOME)/.arduino15/packages/arduino/hardware/avr/$(ARDUINO_SDK_VERSION)/libraries

# Define compiler and programmer settings
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
MCU = atmega328p
prog_MCU = m328p	# avrdude programmer MCU type.
F_CPU = 16000000UL
PORT = /dev/ttyUSB0
PROGRAMMER = arduino
AVRDUDE_FLAGS = -c $(PROGRAMMER) -D -V -b 57600

# Source files
SRC = ILI9327.ino
PREPROCESSED_SRC = $(SRC:.ino=.cpp)
OBJ = $(PREPROCESSED_SRC:.cpp=.o)
HEX = $(PREPROCESSED_SRC:.cpp=.hex)

# Compiler flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DARDUINO=108 -Os -Wall -flto
CXXFLAGS = $(CFLAGS) -std=c++11
LDFLAGS = -mmcu=$(MCU) -flto -Wl,--gc-sections -ffunction-sections -fdata-sections

# Include paths
INCLUDES = -I$(ARDUINO_LIBS)/TFT/src \
		   -I$(ARDUINO_LIBS)/Adafruit_GFX \
		   -I$(ARDUINO_LIBS)/Adafruit_TFTLCD/Adafruit_TFTLCD_Library-1.0.3/ \
		   -I$(ARDUINO_LIBS)/Adafruit_BusIO \
		   -I$(ARDUINO_CORE) \
		   -I$(ARDUINO_LIB)/Wire/src \
		   -I$(ARDUINO_LIB)/SPI/src \
		   -I$(ARDUINO_VARIANTS)/standard \
		   -I./include

# Rules
all: $(HEX)

# Preprocess the .ino file
%.cpp: %.ino
	echo '#include <Arduino.h>' > $@
	cat $< >> $@

# Compile the preprocessed .cpp file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to build core.a locally
core/core.a:
	mkdir -p core
	for ext in cpp c S; do \
		for file in $(ARDUINO_CORE)/*.$$ext; do \
			[ -e "$$file" ] && $(CC) -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -c $$file \
			-I$(ARDUINO_CORE)/ \
			-I$(HOME)/.arduino15/packages/arduino/hardware/avr/1.8.6/variants/standard \
			-o core/$$(basename $$file .$$ext).o; \
		done; \
	done
	ar rcs core/core.a core/*.o

# Compile Adafruit_TFTLCD library
lib/Adafruit_TFTLCD.o:
	mkdir -p lib
	for file in $(HOME)/.arduino15/libraries/Adafruit_TFTLCD/Adafruit_TFTLCD_Library-1.0.3/*.cpp; do \
		$(CC) $(CXXFLAGS) -c $$file $(INCLUDES) -o lib/$$(basename $$file .cpp).o; \
	done

# Compile Adafruit_GFX library
lib/Adafruit_GFX.o:
	mkdir -p lib
	for file in $(HOME)/.arduino15/libraries/Adafruit_GFX/*.cpp; do \
		$(CC) $(CXXFLAGS) -c $$file $(INCLUDES) -o lib/$$(basename $$file .cpp).o; \
	done

# Compile Adafruit_BusIO library
lib/Adafruit_BusIO.o:
	mkdir -p lib
	for file in $(HOME)/.arduino15/libraries/Adafruit_BusIO/*.cpp; do \
		$(CC) $(CXXFLAGS) -c $$file $(INCLUDES) -o lib/$$(basename $$file .cpp).o; \
	done

lib/Wire.o:
	mkdir -p lib
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ARDUINO_LIB)/Wire/src/Wire.cpp -o lib/Wire.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ARDUINO_LIB)/Wire/src/utility/twi.c -o lib/twi.o

lib/SPI.o:
	mkdir -p lib
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ARDUINO_LIB)/SPI/src/SPI.cpp -o lib/SPI.o


# Link the object file and generate the .hex file
$(HEX): $(OBJ) core/core.a lib/Wire.o lib/twi.o lib/SPI.o lib/Adafruit_BusIO.o lib/Adafruit_GFX.o lib/Adafruit_TFTLCD.o
	$(CC) $(LDFLAGS) $(OBJ) core/core.a lib/*.o -o $(PREPROCESSED_SRC:.cpp=.elf)
	$(OBJCOPY) -O ihex -R .eeprom $(PREPROCESSED_SRC:.cpp=.elf) $@

# Upload the .hex file to the Arduino
upload: $(HEX)
	$(AVRDUDE) -v -p $(prog_MCU) $(AVRDUDE_FLAGS) -P $(PORT) -U flash:w:$(HEX):i

# Clean up generated files
clean:
	rm -f *.o *.elf *.hex *.cpp
	rm -rf core/ lib/