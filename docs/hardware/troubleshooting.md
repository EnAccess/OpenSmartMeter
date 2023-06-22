## TITLE: TROUBLESHOOTING MANUAL

### POSSIBLE ERRORS

1. Code not compiling. 
2. Incorrect voltage displaying as compared to a TRMS (True Root Mean Square)
meter. 
3. Incorrect current/power display. 
4. Meter refusing to send data to things board or not connecting to API
(Application Programming Interface). 
5. Keypad not displaying what is pressed correctly. 
6. Incorrect LED (Light Emitting Diode) display

### SOLUTION TO POSSIBLE ERROR

       1. CODE NOT COMPILING: <br>
Code may not compile due to library used in code not installed in the library folder of
the Arduino IDE (Integrated Development Environment) installation path. Developer
should copy the necessary library to the installation path, and restart the IDEthentryre-compiling or install the libraries from

<span style="background: yellow;">**SKETCH** </span>  → <span style="background: yellow;">**INCLUDE LIBRARY** </span> → <span style="background: yellow;">**ADD ZIP LIBRARY** </span>, then add  the necessary library with the complain  on the **verbose** output.


       2. INCORRECT VOLTAGE DISPLAYING AS COMPARED TO A TRMS METER:

Meter may display incorrect voltage if the developer did not tune the variable resistor
to calibrate the voltage in comparison with a TRMS meter or another developer maychoose to change the calibration formula on the firmware to display the voltagecorrectly. The calibration formula can be seen in the <span style="background: yellow;">void measure() under mains_input_value=mains_v*0425560128029263831732967543667. </span>


        3. INCORRECT CURRENT/POWER DISPLAY:

Meter may display wrong current/power if not properly calibrated according tothetransformation ratio and if the current transformer is not in-line with the burdenresistor based on the below formula :

**Burden resistor = (Aref *CT turns) / (2√2 *CT current)**

Where :<br>
Aref = analog reference voltage(5v)<br>
CT turns= current transformer turn ratio<br>
CT current = current transformer maximum current handling capability. <br>
<u>Note:</u> the resistor value does not have to be exact, but close to the calculated value. After doing this and true value is not gotten as compared to a current measuring meter, developer will need to re-calibrate from the formula available    <span style="background: yellow;">void measure() under curr=relcur*0.02545454545454545454545454545454</span>
  to get desired current as
measured and displayed by the current measuring meter or device, which inturnaffect the power displayed.


       4. METER REFUSING TO SEND DATA TO THINGS BOARDORNOTCONNECTING TO API:

Change the APN (Access Point Name) to match the APN of the Simcard used, alsocheck to make sure there is match between the things board token on firmware andthings board page token created. 

       5. KEYPAD NOT DISPLAYING WHAT IS PRESSED CORRECTLY:

If meter does not display what is pressed correctly on the LCD while inputtingSTStoken, developer should reverse keypad connection on hardware. 

       6. INCORRECT LED DISPLAY:

If LED display incorrectly for low/normal credit, developer can change hardwareconnection to LED or change code pin assignment between green LEDand red LEDon #define <span style="background: yellow;">red_led and #green_led</span>.