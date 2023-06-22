## TITLE: PRODUCT AND SPECIFICATION MANUAL

### 1. Introduction
This is a meter with RS232 and wireless GSM communication using Cicada GSMmodule. It has various event alerting functions including kWh remote readingandlogging, using remote management and tampering provides customers a convenient
remote reading method, on the air top-up. This meter is favored by customers andmanufacturers with the features of high accuracy, wide measuring range andcompetitive cost performance. 

### 2. Tariff
Tariff is software adjustable by energy provider as desired for meter. 

### 3. Communication
Meter support RS232/2G/3G GSM, baud-rate: 115000. 

### 4. Measurement
Meter measures voltage, current, active power and voltage is displayed on the 16x2LCD, accumulative power and top-up history is logged every 24 hours ontheweb-based software.

### 5. Mode
Meter has 2 operation modes:

1. On the air top-up. 
2. STS token mode using keypad. 

#### Over the air top-up
Over the air top-up is done via the web-based software available, with meter tobetopped up pre-registered with a unique meter-id, the rest is handled betweenthesoftware and firmware to top-up the corresponding meter whose meter-id is enteredautomatically. 

#### STS operation
Enter 20-digit token via keypad available on meter, the token is generated fromthewebsite in event of network failure or malfunction.

### 6. Technical Parameter

| DESCRIPTION |                   SINGLE PHASE VALUE |
| -------- | -------- |
| Accuracy    |                   **CLASS 1**    |
| **Voltage** <br>   Reference voltage <br> Operating voltage range |                   <br>                   240 V   <br>                   70% - 120% Reference voltage    |
| Current    |                   5 (80A)    |
| Frequency    |                   50Hz    |
| **Temperature** <br> Operating range <br> Limit range for storage and transport   | <br>                   - 250C to 700C <br>                   -400C to 1000C    |
| **Power consumption** <br> Power consumption in voltage circuit(active) <br> Power consumption in voltage circuit(apparent) <br> Power consumption in current circuit   | <br>                   2W <br>                   10VA <br>                   1VA    |
| Communication Interface    |                   DLMS / COSEMCompliance <br>                   (GPRS)First Electric web API    |
| Dimension    |                   18cm x 15cm    |
| Display:    |                   LCD 16x2   |


**Indicator**

|  |  |  |
|  | |  |
| 1    | Credit Indicator    | Bi-color LED (red) <br> Green LED light on when credit is in highlevel <br> Red LED light on when credit is low <br> Red LED blinks when credit has finished   |
| 2    | Alarm indicator    | The indicator blinks when event occur, <br> event suchas overload, low voltage, lowcredit or creditexhausted.    |


### 7. Communication

1. RS232 communication, used for local reading/monitoring. 
2. GPRS communication, used for on the air top-up, remote monitoringandcontrolling.


### 8. Display of details on LCD

![meter]('../../../assets/hardware/meter.png)


### 9. Safety Instructions
**Information for Your Own Safety** <br>
This manual does not contain all the safety measures for operation of this equipment
(module, device) because special operating conditions, local code requirements or
local regulations may necessitate further measures. 

### 10. Qualified personnel
Installation and operation, assembling of this equipment described in this manual mayonly be performed by qualified personnel. Only people who have good knowledge onpower/embedded system should assemble this equipment.
