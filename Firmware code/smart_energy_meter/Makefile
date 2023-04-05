# Define the board and the serial port you are using
BOARD_TAG = "STM32:stm32"
SERIAL_PORT = "/dev/ttyACM0"
# Define the FQBN
FQBN = "STM32:stm32:GenF1:pnum=BLUEPILL_F103C8"

# Define the URLs of the platform libraries you want to install
PLATFORM_URLS = \
	https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json

# Define the command to install platform libraries
ARDUINO_PLATFORM_UPDATE = arduino-cli core update-index
ARDUINO_PLATFORM_INSTALL = arduino-cli core install $(BOARD_TAG)

# Parse the library dependencies from platformio.ini to Arduino compatible format
PLATFORMIO_LIBS = $(shell sed -n '/^lib_deps *= *$$/,/^$$/p' platformio.ini | sed '1d;$d;s/ //g;/^http/!s/^[^/]*\///')

.PHONY: print-platformio-libs
print-platformio-libs:
	@echo $(PLATFORMIO_LIBS)

.PHONY: install
install:
	# Install platform libraries
	$(ARDUINO_PLATFORM_UPDATE) $(foreach url,$(PLATFORM_URLS),--additional-urls $(url))
	$(ARDUINO_PLATFORM_INSTALL) $(foreach url,$(PLATFORM_URLS),--additional-urls $(url))

	# Install libraries from GitHub
	$(foreach lib,$(filter %.git,$(PLATFORMIO_LIBS)),ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true arduino-cli lib install --git-url $(lib);)

	# Install libraries from the Arduino Library Manager
	arduino-cli lib install $(filter-out %.git,$(PLATFORMIO_LIBS))

.PHONY: compile
compile:
	# Compile the project
	arduino-cli compile --fqbn $(FQBN) .

.PHONY: upload
upload:
	# Upload the project to the board
	arduino-cli upload -p $(SERIAL_PORT) --fqbn $(FQBN) .

.PHONY: build
build: install compile
