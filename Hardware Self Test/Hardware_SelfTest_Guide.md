# Hardware Self Test Guide

This Guide describes how the Hardware Self Check is performed
The firmware checks if the following Peripherals are connected on the board: LCD, LEDs(Green and Red), Buzzer(Passive), Relay, EEPROM, RTC, GSM Modem, Keypad and UART Port
It runs in a sequential manner and the procedure for verifying each Peripheral is described below.

1. **LCD**

The LCD is a 16x2 display. It comes up and displays **HARDWARE** on the first row and **SELF-TEST** on the second row. The LCD continues to display as each peripheral test is being executed.  

2. **Buzzer**

The Buzzer is a passive one and a tone is generated at a frequency of 500Hz and for a duration of 2 seconds.

3. **Relay**

The L293D H Driver is used to drive the relay of the meter. The Relay is tested by turning it ON for 2 seconds and then OFF.

4. **LEDs**

The Red LED comes ON for 1 second and then goes OFF. The Green LED also comes ON for 1 second and then goes OFF.

5. **EEPROM**

The EEPROM(AT24C32) is connected to the MCU via I2C and acts as a slave on the I2C bus. The MCU initiates an I2C communication with the address of the EEPROM, if the EEPROM responds with an ACK signal, then it is present and the LCD displays **EEPROM FOUND**. If not, the LCD displays **EEPROM NOT FOUND**

6. **RTC**

The RTC(DS1307) is connected to the MCU via I2C and also acts as a slave on the I2C bus and it is also on the sae bus as the EEPROM. The MCU initiates an I2C communication with the address of the RTC, if the RTC responds with an ACK signal, then it is present and the LCD displays **RTC FOUND**. If not, the LCD displays **RTC NOT FOUND**

7. **GSM Modem**

The GSM Modem(SIM800C) is connected to the MCU via UART and the MCU tries to communicate with it by trying different baud rates. If it is successful, the LCD displays **MODEM FOUND**. If not, **MODEM NOT FOUND** is displayed on the LCD.

8. **Keypad** 

The LCD displays a prompt: **PRESS KEYPAD**. Any key pressed on the 4x4 Keypad is displayed on the LCD.

9. **UART Port**

The UART Port echoes back any input it gets.