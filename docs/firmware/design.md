# Firmware Design

## Design Objective:

- LCD (Liquid Crystal Display) display to show periodic energy consumption, credit remaining and tariff.
- Interface to Cicada communication module assuming GSM connection.


Design specification and justification
In the design of the GSM based prepaid energy meter, several meter designs were investigated, in other to have an historical experience on energy meter design in terms of design flaw, topology, advantage of one design over the other, NEMSA certification design guide/rule.

### LCD DISPLAY

LCD was incorporated into the firmware code, to enable easy physical communication between users and meter, information can be sent from micro-controller to LCD either using the full 8bit data line or in nibble using 4bit, in this design, data is transmitted from the micro-controller to the LCD in nibbles, as this helps to save the micro-controller 4 extra pins, also the speed difference between 8bit data transmission and 4bit data transmission is in micro-seconds, which is negligible for application such as information display. The micro-controller communicates with the 16x2 LCD screen using M2M (Micro-controller to Micro-controller) I2C (Inter Integrated Circuit) serial communication protocol. This protocol enable communication with several M2M communication, where the micro-controller is the master and the other micro-controller which in our case LCD is the slave. Only the address line and data line (D4, D5, D6, D7) for LCD is needed to tell the address of the slave micro-controller for a particular data sent by the master. Arduino LCD library was used to send information to the 16x2 LCD screen.
