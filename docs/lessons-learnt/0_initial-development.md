The lessons learnt in the development of the smart energy meter have been split into hardware and firmware sections below:

## Firmware

### Meter calibration

It was discovered during the meter calibration process that the power displayed from the chip power measurement register is not always correct.
To solve this, we had to manually multiply out the values of voltage, current, PF in application to get correct value of power.

### Unavailability of Own Test Bench

A key step in the meter design involves calibration of the meter.
This was a little challenging as we did not own a test bench. We had to write to several labs that would give permission for us to use their test bench for meter calibration.
This resulted in some delays to the project.

### External EEPROM/RTC module

The RTC module has on it an external EEPROM chip which is used to save vital information.
It was discovered that if the RTC time parameters are not preset before using it, the RTC will fail to communicate with the microcontroller via SPI.
It was also discovered that the location the developer chooses to write the non-volatile information to must be pre-written to before trying to fetch and save information, else the saved information will get corrupted as the microcontroller will fetch negative ambiguous value, this is because by default, the EEPROM locations are not assigned a value of zero, rather they have default negative values.

## Hardware

### Electrical noise

The accuracy of the current values measured by the AFE (Analog Front End) chip is affected by electrical noises especially at very low current below 0.5Amps, either the design engineer comes up with solutions to eliminate this electrical noise or rewrite a calibration formula for low current measurement error compensation.
It was also discovered that the current sensor picked up electrical noises at low current if the current sensor extension cable is long or not twisted together.

### External EEPROM

The used micro-controller (`STM32F103`) has no internal EEPROM, this created a big issue for us as its important and vital to have important information saved up in a non-volatile memory like EEPROM, several approaches were developed such as modulating EEPROM on the flash memory of the micro-controller, this worked however, it had some flaws with the originality of the microcontroller.
This EEPROM modulation method proved futile on some clone or inferior chips which left the team in a difficult situation. Lucky enough, the RTC module had an external but powerful EEPROM chip (`AT24C32`) on it which was eventually used.
The EEPROM had multiple locations with 100years information retainability and several other interesting features.
The AT24C32 was used to save vital information.

### Network issue with Cicada GSM module

The development team noticed a network modulation issue on the module used if an antenna is not connected to the antenna connector, we suspect this is due to the module not having an on-board PCB antenna.
In summary, to get a good network reception while running tests, it is advisable to connect an antenna to the antenna connector.

### SIM card damage issue

It was discovered that the simcard inserted into the sim slot gets damaged if the module is powered and the simcard is not fully inserted for whatever reason.

### MOSFET getting blown in SMPS power supply stage

Generally, zero valued MOSFET should not be used for fast switching power electronics devices.
We recommend adopters ensure the MOSFET used in the power supply stage is of high quality as this may result in MOSFET getting blown or heating up if it is discovered to be of inferior quality.

### Chopper getting blown in power supply stage

The chopper in the power supply stage may get blown if wrong capacitor values are used, especially if the frequency selecting resistor/capacitor is wrongly placed or if wrong values of stabilizing capacitors are used.

### Keypad pressing itself

It was discovered that if low quality membrane keypad is used, the keypad was performing a self-pressing action without anybody physically pressing any key.
Adopters should ensure good quality keypads are used.

### Information not received via UART port

Information may not get to the PC while trying to fetch metering parameters via RS232 if the RS232 module is bad or the used cable is of low quality.

### Supply Chain

Some key components such as the AFE chip were not available locally and had to be sourced from China, this resulted in some delays.
