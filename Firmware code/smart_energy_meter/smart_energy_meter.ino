#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

HardwareSerial Serial2(PA3, PA2);
   #include <SAM_UART.h>
   HardwareSerial ATM90E26(PB11, PB10);
   ATM90E26_UART AFE_chip(&ATM90E26);


   //keypad
   #include <Keypad.h>
   #define Lengths 20
   char Data[Lengths];
   int lcd_count, convertedsts_data = 0;
   byte data_count, data_count2, dt, sts_accept, encoder = 0;
   char customKey;
   const byte ROWS = 4;
   const byte COLS = 4;
   char hexaKeys[ROWS][COLS] = {
   {'1', '2', '3', 'A'},
   {'4', '5', '6' , 'B'},
   {'7', '8', '9', 'C'},
   {'*', '0', '#', 'D'}
   };
   byte rowPins[ROWS] = {PB9, PB8, PB1, PB0};
   byte colPins[COLS] = {PA7, PA6, PA5, PC13};
   Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);



   #define TINY_GSM_MODEM_SIM800
   #define TINY_GSM_RX_BUFFER   60  // Set RX buffer to 1Kb
   #include <TinyGsmClient.h>
   #include <ArduinoHttpClient.h>
   #include "ThingsBoard.h"

   const char apn[]  = "terminal";
   const char user[] = "";
   const char pass[] = "";
   
   #define TOKEN               "5GBw6kqNCN93BN3nuuvJ"  //"YOUR_ACCESS_TOKEN"
   #define THINGSBOARD_SERVER  "demo.thingsboard.io"
   #define THINGSBOARD_PORT    80
   
   TinyGsm modem(Serial1);
   TinyGsmClient client(modem);
   ThingsBoard tb(client);
   bool modemConnected = false;


    // SIM card PIN (leave empty, if not defined)
    const char simPIN[]   = "";

    const char server[] = "paygotesting.000webhostapp.com"; // domain name: example.com, maker.ifttt.com, etc
    const char resource[] = "/subscribe.php";         // resource path, for example: /post-data.php
    const int  port = 80;                             // server port number
    HttpClient http(client, server, port);
    String apiKeyValue = "tPmAT5Ab3j7F9";


   unsigned int over_voltage = 250;
   unsigned int over_load = 25000;
   unsigned int low_voltage = 150;
   unsigned int low_freq = 45;

   unsigned int tariff = 70;
   String stat = "sucess";
   String sts_data = "";
   String sts_data1 = "";
   int private_stskey = 109;



   #include <Wire.h> 
   #include "RTClib.h"
   RTC_DS1307 rtc;
   char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
   unsigned int hours, minutes, seconds, rtcday = 0;
   #include <AT24CX.h>
   // EEPROM object
   AT24CX mem;

   #include <LiquidCrystal.h>
   const int rs = PB3 , en = PA15, d4 = PA8, d5 = PB15, d6 = PB14, d7 = PB13;
   LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


   #define buzzer        PB5
   #define relaya        PB12
   #define relayb        PB4

   #define red_led       PA0
   #define green_led     PA4
   #define battery_input PA1



   unsigned int encodernew  =  0;
   unsigned long eepromupdate_time, eeprom_sts_data, prev_energypulse, new_energypulse, topupnew    = 0;
   float   deduction_factor, topup, creditt = 0.0;
   float  lcd_creditt, totalbill, energy_billing, dcode, ENERGY, ENERGY2, powerFactor, freq, btValue  = 0.0;
   float  prevpulsecounttime, pulsetiming, pulsecounttime = 0.0;
   unsigned long LCD_scroll_time = 5000; // 10sec according to Nigeria metering code
   unsigned long lcdtime_now, prev_lcdtime = 0;
   unsigned long   lcd_refresh, lcd_reset, mains_inputV, cnt, p_time   = 0;
   unsigned long  set_inv_wattage, pv_v, u, mains_v, j, h, m,  sts_mode = 0;
   float  cu, dc, t_cu, btt, currinv_adc, currmains_adc, p_mains, curr_mains_Value, curr_inv_Value, init_curr, billing, curr, true_power2, currnew = 0.0;
   float  pvvalue, mainsvalue, curinv_v, curnep_v, relcur, but, bat_curr, bat_curr_value, p_inv, inv_power, mains_power  = 0.000;
   byte internetpost_time, internetget_time, internetenergy_time,  credit_time, sucess, parameters = 0;
   unsigned int pulse, pulse_set  = 0;
   unsigned int pulse_delay = 200;
   unsigned long sts_value, convertedsts_day, pulsetime,  current_time, previous, previousenergytime, previousenergytime2, energytime, energytime2, currentenergytime, currentenergytime2  = 0;
   byte get_credit, fault, fault_written= 0;
   byte off_exec=0;
   byte on_exec=0;
   int confirmkey, tamper_log = 0;
   byte token_used, thingsboard_check = 0;
   
   
   float true_power,mains_input_value, lastmonth_KWH  = 0.0;
   float pulsetime_now, prev_pulsetime =  0.0;
   long warn_now = 0; 
   int tamper_location =5;
   long warntime, nw_month_cnt, rtcmonth, rtcnewmonth, billing_date=0;
   byte c_chek, tp_fetch, token_ok  =0 ;

   unsigned  long token_eeprom_location = 20;
   unsigned long eeprom_location_cnt   = 40;
   unsigned long credit_eeprom_location = 9;
   

    
 
//    for meter  
   String meter="MT100";
   unsigned long meter_no=100;
   unsigned long multiplier = 100; // to raise meter no to 5 digit
   unsigned int  meter_no_count = 3;
   unsigned long sts_eeprom_fetched=0;
 

   byte fe1[8] = {
   0b00011,
   0b00011,
   0b00011,
   0b00011,
   0b00011,
   0b11111,
   0b11111,
   0b11111
   };

   byte fe3[8] = {
   0b11111,
   0b11111,
   0b11111,
   0b00011,
   0b00011,
   0b00011,
   0b00011,
   0b00011
   };

   byte fe2[8] = {
   0b11111,
   0b11111,
   0b00000,
   0b00000,
   0b00000,
   0b01100,
   0b01100,
   0b01111
   };

   byte fe4[8] = {
   0b01111,
   0b01100,
   0b01100,
   0b00000,
   0b00000,
   0b00000,
   0b00000,
   0b00000
   };


//----SETUP
void setup() {

  lcd.begin(16, 2);
  pinMode (buzzer,         OUTPUT);
  pinMode (relaya,         OUTPUT);
  pinMode (relayb,         OUTPUT);
  pinMode (red_led,        OUTPUT);
  pinMode (green_led,      OUTPUT);
  
  digitalWrite(buzzer,   HIGH);


  lcd.createChar(0, fe1);
  lcd.createChar(1, fe2);
  lcd.createChar(2, fe3);
  lcd.createChar(3, fe4);
  lcd.setCursor(0, 0);
  lcd.print("  DEVELOPED BY  ");
  lcd.setCursor(0, 1);
  lcd.print(" FIRST ELECTRIC ");

  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.write(byte(0));
  lcd.setCursor(8, 0);
  lcd.write(byte(1));
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  lcd.setCursor(8, 1);
  lcd.write(byte(3));
 
  ATM90E26.begin(9600);
  AFE_chip.SET_register_values();
  delay(1000);
  digitalWrite(buzzer, LOW);
 
  Serial2.begin(115200);
  Serial2.print("working");
  Serial1.begin(115200);
  delay(3000);
  Serial1.write("AT+IPR=9600\r\n");
  Serial1.end();
  Serial1.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print(" SYSTEM BOOTING ");
  lcd.setCursor(0, 1);
  lcd.print(" ##             ");

  Serial2.println("Initializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  lcd.setCursor(0, 0);
  lcd.print("                     "); 
  lcd.setCursor(0, 1);
  lcd.print("                    "); 
   
  Serial2.print("Modem: ");
  Serial2.println(modemInfo);
  lcd.setCursor(0, 0);
  lcd.print(" modemInfo "); 
  lcd.setCursor(0, 1);
  lcd.print( modemInfo ); 
  delay(2000);
  lcd.clear();

  if (! rtc.begin()) {
  lcd.print("Couldn't find RTC");
  delay(2000);
  while (1);
  }
  if (! rtc.isrunning()) {
  lcd.print("RTC is NOT running!");
  delay(2000);
  }

  lcd.setCursor(0, 0);
  lcd.print("CSOne :         ");
  lcd.setCursor(8, 0);
  lcd.print( AFE_chip.FETCH_MeterCSOne() );

  lcd.setCursor(0, 1);
  lcd.print("CSTwo :         ");
  lcd.setCursor(8, 1);
  lcd.print( AFE_chip.FETCH_MeterCSTwo() );
  delay(1000);
  
  // rtc eeprom
  //mem.writeLong(credit_eeprom_location, 200);
//  mem.writeLong(eeprom_location_cnt, token_eeprom_location);
//  delay(20);
//  mem.writeLong(token_eeprom_location, 200);
//  delay(20);
  creditt =  mem.readLong(credit_eeprom_location);
  if ( creditt >= 0 ) {
  mem.writeLong(credit_eeprom_location, creditt);
  }
  delay(10);
  relay_on ();

 
  
  #if defined (TIM1)
  TIM_TypeDef *Instance = TIM1;
  #else
  TIM_TypeDef *Instance = TIM2;
  #endif
  HardwareTimer *MyTim =new HardwareTimer(Instance);
  MyTim -> setOverflow (20,HERTZ_FORMAT);   
  MyTim -> attachInterrupt (urgeent);
  MyTim -> resume();
}

void loop() {

   mesure();
   if ( (mains_input_value > 50) ) {
   credit_reminder();
   }
   if ( (mains_input_value < 50) ) {
   digitalWrite(red_led, LOW);
   digitalWrite(green_led, LOW);
   }
   get_time();
   if ( (sts_mode == 0) && (mains_input_value > 50) ) {
   gsm_func();
   }
}
