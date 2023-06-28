## TITLE: PCB BOARD DESIGN
### Design Objective
- Cicada GSM module connection to PCB (Printed Circuit Board).
- RTC (Real Time Clock) board connection to PCB. RTC will be a different boardconnected to PCB of the meter.
- 16x2 LCD (Liquid Crystal Display) connection to PCB.
- Power board design on same PCB.
- Variable resistor for Vrms calibration purpose
- Hall effect current sensor.
- Electromagnetic relay.
- RS232 port.
- STM32 based design.
- 2-layer PCB CAD design on Kicad
- Circuit layout design.
- Gerber production file.

### Design Rationale

### 1. Cicada GSM module connection to PCB

In the design of the GSM based energy meter, Cicada 2G/3G GSMwas selectedasthe GSM communication module to be used send and receive information fromtheweb application due to its reliability/stability as needed for communication purposeinthis prepaid energy meter design. The cicada pin-out connection also features cicadaWIFI module to be connected to same pin-out on the PCB to remove network barrier
and limitation in areas with poor GSM connection.

### 2. RTC

RTC(Real Time Clock) module is needed in this design to keep track of time, alsoit
helps in timing the uploading of energy and credit information fromthe hardware tothe things-board web application, also the RTC module serves as a stable time basetokeep track of all time based operation in the meter, operations such as STS Top-upvalidation, energy tracking and more, an RTC module was used as opposed to usingan RTC chip, to reduce manufacturing error due to closeness of crystal oscillator tothe chip and other minor RTC manufacturing error, which pose accuracy threat tothedesign, also to ease the process of manufacturing and aid compactness. The RTC
2
module also has an EEPROM chip named AT24C32, this chip will help save all
metering information such as STS token, tariff and much more as needed.

### 3. 16x2 LCD

A 16x2 LCD from Hitachi is proposed and used in this design to display all necessaryparameters such as RMS voltage, current, power, energy, Top-up balance and visuallysee STS token number inputted for manual Top-up in event of network failure, thisLCD serve as physical interactive medium between electrical parameter measuredanduser readable and understandable information.

### 4. Power board design

The power board design for the energy meter is a SMPS (Switch Mode Power Supply)
based design with a fly back forward switching topology using UC3843 current
controller IC and ferrite core transformer, this topology was selected as opposedtousing transformer based design due to its reliability, weight, cost, sophisticationandperformance under low voltage/high voltage conditions without failure, also voltageregulator IC(Integrated Circuit) were used to regulate voltage as needed for the GSMmodule, micro-controller and other module needing regulated voltage, voltageregulator IC such as (L7805, LM350) were used in regulation, data-sheet to each ICisavailable on the data-sheet section of the deliverable set1.

### 5. Variable resistor calibration for VRms

 Variable resistor is incorporated into this design to enable calibration the ACvoltageas required.

### 6. Electrically isolated hall effect sensor

The current sensing method used here had to be changed from hall effect current
sensor to current transformer measuring method, this method is used to increaseefficiency. An accuracy test between hall effect current sensor and current transformer
was performed and it was discovered that current transformer provides better
accuracy and stability when measuring AC current as compared to hall effect current
sensor. In addition, hall effect current sensors accuracy is affected by heat and moresusceptible to electromagnetic interference due to the sensors electromagnetic diesensing method used as proposed by **“Edwin Hall”**.

### 7. Electromagnetic relay to turn users on/off

Magnetic latching relay is used in place of electromagnetic relay due to cost,
3
reliability, power consumption, size. Comparison table is highlighted below.

| Electromagnetic relay |             Magnetic latching relay|
| -------- | -------- |
| Terminal is susceptible to arching and <br> carbon  which eventually lead to relay failure.                                     |                Terminal is less susceptible to arching and <br>             carbon which eventually leadtorelay.    |
| High cost in comparison to current <br> handling capability.   |             Low cost in comparison to current <br>             handling capability.   |
| Takes a lot of space.   |             Takes a very small considerable space.   |
| Continuous supply power is needed <br> to keep relay active   |             A single pulse of power is only needed <br>             to trigger the relay on or off.   |

 The above advantage of magnetic latching relay as compared to electromagnetic relayin terms of **cost, size, terminal carbonizing, power consumption and current
handling capability** is why we opted for a magnetic latching relay as opposedtousing an electromagnetic relay.

### 8. STM32 based design

**Stm32f401cc** was the first chip proposed to be used in the design of the energy meter
due to its speed, cortex-M4 architecture and other feature, however the chiphasinternal oscillator regulator issue during upload, which makes upload a little bit tasky, in view of that, we had to switch to a more stable chip **STM32f103C8T6**. STM32 based chip was used in the control of all operations in the meter, the STM32micro-controller used in the energy meter design is **STM32f103C8T6**, this chipwasselected due to its ease of use, firmware upload to the chip is not affected by heat, stability, 32bit processing speed, arm cortex M3 incorporation, ART accelerator, FPU(floating point unit calculation), zero wait state, available break out boardfor
easy manufacturing called **“blue pill”**, add-on feature to use the popular Arduino-unoIDE to write, compile and upload code to the micro-controller via USBandSTM32-Cube-MX. More information about the feature of the chip is available inthedata-sheet available on the En-Access energy meter Github page.

### Deliverables
Deliverables such as 2-layer PCB board, circuit design layout, Gerber productionfilesare available on the Github folder named **Deliverable Set 2**
