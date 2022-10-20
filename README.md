# Open Smart Energy Meter

This repository contains manufacturing details as related to smart energy meter, details such as hardware design, web software, firmware, exterior casing STL files, rationale behing design, how-to document to guide a skilled user to replicate every part of the smart meter is available in the repository.

  ![EnAcess PCB new](https://user-images.githubusercontent.com/99180312/196974871-8cfb9366-eee0-4abf-8d39-d806d01217dd.png)


 

## Technical developmental breakdown
The open-source smart meter development has four main parts to it as highlighted below:

### Circuit and Enclosure Design :
  The circuit was designed on Kicad free and open-source PCB design software while the enclosure design was done using Fusion 360 software. 

### PCB and Enclosure fabrication :
  PCB fabrication involves putting together the physical components that makes up the smart meter. The PCB features a 2layer design incorporating CICADA GSM module for communication while the enclosure offers a minimalistic IP 51 design according to IEC metering enclosure design standard. 

### Firmware, Web interface Development & API integration : 
  The firmware code was written in Arduino IDE using C++, while the web front end was written majorly in HTML and CSS while the back end was written majorly in PHP. Links to all these codes are here. Thingsboard API was used to view meter parameters such as energy, credit. 

### Calibration & Testing :
  After putting the hardware and software together, we needed to validate the performance of the smart meter and ascertain its accuracy. Due to varying tolerances of used components, it is vital to perform a calibration exercise. This is used to correct any errors due to varying tolerances. Further details including source code, schematics, and manufacturing files can be found in the EnAccess github repository
  
## Repository structure 
#### documentations : 
The documentation folder contain all information as related to manufacturing the smart energy meter, information such as Api documentation , rationale behind design, calibration report, trouble shooting manual, bill of material, datasheet for all components used. 
#### Firmware codes : 
The firware code folder contain the firmware code written in C++ on Arduino platform, the folder also contain the library files used in the code. 
#### Meter hardware design :
The meter hardware design folder contain the Kicad design file, gerber manufacturing file, exterior casing manufacturing files, design images. 
#### Web software demo video : 
This folder contain the web demo video to guide a developer developer on the functionality of the web software.
#### Web software:
This folder contain the web software codes used. the link to the web demo is  : https://paygotesting.000webhostapp.com

## Quick look at the schematic

#### Microcontroller voltage regulation stage : 
The voltage regulation stage contains the 7805IC to regulate the voltage from the SMPS power supply to 5V as this is the safe voltage level to power the microcontroller, also this stage contains the LM350IC to regulate the SMPS power sully to 4V to power the GSM module.  

![image](https://user-images.githubusercontent.com/99180312/195889249-82d45939-8c51-4948-8c72-a401e8434b23.png)


#### Analog front end chip connection : 
The analog front end chip (AFE) is a metering IC suitable for a class1 and class2 accuracy meter and fit for use according to the relevant metering standard as specified by IEC. it is used in this design to measure power, voltage, current, power factor e.t.c.

![image](https://user-images.githubusercontent.com/99180312/195888941-8220682d-cc43-435a-833b-c85cde8cb5fb.png)


#### SMPS power supply stage : 
The SMPS stage converts the AC input voltage supplied into the meter to 12V DC to power the internal electronics circuit.

![image](https://user-images.githubusercontent.com/99180312/195889282-7c816822-8ac1-4781-a278-c50e86c304fc.png)



#### Microcontroller/Cicada GSM Module/LED/ LCD\n RS232/RTC/Relay/Keypad/Buzzer 
The master of this stage is the microcontroller(STM32f103ccu8) popularly known as bluepill, it fethes outputted meter values from the AFE chip via the USART communication interface, it also communicate with the webpage via the Cicada GSM module and sends neccesary meter parameters to the RS232 port, this stage also interface with other hardware pheripherials such as keypad, buzzer, magnetic latching relay, LED, LCD.
 

<img width="475" alt="Screen Shot 2022-10-14 at 2 33 33 PM" src="https://user-images.githubusercontent.com/99180312/195888904-0bd73e82-5dbd-4e26-bc52-04fa8206d831.png">

