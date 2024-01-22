# Troubleshooting

## TROUBLESHOOTING MANUAL

### Possible errors

1. Code not compiling.
2. Incorrect voltage displaying as compared to a TRMS (True Root Mean Square) meter.
3. Incorrect current/power display.
4. Meter refusing to send data to ThingsBoard or not connecting to API (Application Programming Interface).
5. Keypad not displaying what is pressed correctly.
6. Incorrect LED (Light Emitting Diode) display

### Solution to possible errors

#### Code not compiling

Code may not compile due to library used in code not installed in the library folder of
the Arduino IDE (Integrated Development Environment) installation path.
Developer should copy the necessary library to the installation path, and restart the IDE then try re-compiling or install the libraries from

==**SKETCH**== → ==**INCLUDE LIBRARY**== → ==**ADD ZIP LIBRARY**==, then add the necessary library with the complain on the **verbose** output.

### Incorrect voltage displaying as compared to a TRMS meter

Meter may display incorrect voltage if the developer did not tune the variable resistor
to calibrate the voltage in comparison with a TRMS meter or another developer maychoose to change the calibration formula on the firmware to display the voltage correctly.

The calibration formula can be seen in the ==void measure()== under ==mains_input_value=mains_v\*0425560128029263831732967543667==.

#### Incorrect current/power display

Meter may display wrong current/power if not properly calibrated according to the transformation ratio and if the current transformer is not in-line with the burdenresistor based on the below formula :

**Burden resistor = (Aref *CT turns) / (2√2 *CT current)**

Where :<br>
Aref = analog reference voltage(5v)<br>
CT turns= current transformer turn ratio<br>
CT current = current transformer maximum current handling capability. <br>

:::info note
the resistor value does not have to be exact, but close to the calculated value.
After doing this and true value is not gotten as compared to a current measuring meter, developer will need to re-calibrate from the formula available ==void measure()== under ==curr=relcur\*0.02545454545454545454545454545454== to get desired current as measured and displayed by the current measuring meter or device, which inturnaffect the power displayed.
:::

#### Meter refusing to send data to ThingsBoard or not connecting to API

Change the APN (Access Point Name) to match the APN of the Simcard used, also check to make sure there is match between the ThingsBoard token on firmware and ThingsBoard page token created.

#### Keypad not displaying what is pressed correctly

If meter does not display what is pressed correctly on the LCD while inputting STStoken, developer should reverse keypad connection on hardware.

#### Incorrect LED display

If LED display incorrectly for low/normal credit, developer can change hardware connection to LED or change code pin assignment between green LED and red LED on

```cpp
#define red_led
#define green_led
```
