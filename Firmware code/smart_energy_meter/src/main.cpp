// defines

// Arduino base libraries
#include <Wire.h>
#include "Arduino.h"

// third party libraries
#include <ArduinoHttpClient.h>

// OpenSmartMeter libraries
#include "credit.hpp"
#include "global_defines.hpp"
#include "helpers.hpp"
#include "lcd_display.hpp"
#include "lcd_init.hpp"
#include "mem_init.hpp"
#include "power.hpp"
#include "relay.hpp"
#include "remote.hpp"
#include "sts_token.hpp"
#include "thingsboard.hpp"
#include "time_management.hpp"
#include "token_management.hpp"


extern "C" {
#include "opaygo_decoder.h"
}

HardwareSerial Serial2(PA3, PA2);

byte fe1[8] = {0b00011, 0b00011, 0b00011, 0b00011,
               0b00011, 0b11111, 0b11111, 0b11111};

byte fe3[8] = {0b11111, 0b11111, 0b11111, 0b00011,
               0b00011, 0b00011, 0b00011, 0b00011};

byte fe2[8] = {0b11111, 0b11111, 0b00000, 0b00000,
               0b00000, 0b01100, 0b01100, 0b01111};

byte fe4[8] = {0b01111, 0b01100, 0b01100, 0b00000,
               0b00000, 0b00000, 0b00000, 0b00000};

void setup() {
  lcd.begin(16, 2);
  pinMode(buzzer, OUTPUT);
  pinMode(relaya, OUTPUT);
  pinMode(relayb, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);

  digitalWrite(buzzer, HIGH);

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
  lcd.print(modemInfo);
  delay(2000);
  lcd.clear();

  if (!rtc.begin()) {
    lcd.print("Couldn't find RTC");
    delay(2000);
    while (1)
      ;
  }
  if (!rtc.isrunning()) {
    lcd.print("RTC is NOT running!");
    delay(2000);
  }

  lcd.setCursor(0, 0);
  lcd.print("CSOne :         ");
  lcd.setCursor(8, 0);
  lcd.print(AFE_chip.FETCH_MeterCSOne());

  lcd.setCursor(0, 1);
  lcd.print("CSTwo :         ");
  lcd.setCursor(8, 1);
  lcd.print(AFE_chip.FETCH_MeterCSTwo());
  delay(1000);

  // rtc eeprom
  // mem.writeLong(credit_eeprom_location, 200);
  //  mem.writeLong(eeprom_location_cnt, token_eeprom_location);
  //  delay(20);
  //  mem.writeLong(token_eeprom_location, 200);
  //  delay(20);
  creditt = mem.readLong(credit_eeprom_location);
  if (creditt >= 0) {
    mem.writeLong(credit_eeprom_location, creditt);
  }
  delay(10);
  relay_on();

#if defined(TIM1)
  TIM_TypeDef* Instance = TIM1;
#else
  TIM_TypeDef* Instance = TIM2;
#endif
 HardwareTimer* MyTim = new HardwareTimer(Instance);
  MyTim->setOverflow(20, HERTZ_FORMAT);
  switch (Mode_select)
  {
  case 0:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("No Configuration! ");
    while(Mode_select == 0) // wait for mode configuration
    {
      STS_keypad();
      delay(20);
    }
    break;

  case 1:
    MyTim->attachInterrupt(urgeent);
    MyTim->resume();
    break;
  
  case 2:    
    /*OpenPayGo Token initializing code; */
    break;
  }
}

void loop() {
  if(Mode_select == 1)
  {
    mesure();
    if ((mains_input_value > 50)) {
      credit_reminder();
    }
    if ((mains_input_value < 50)) {
      digitalWrite(red_led, LOW);
      digitalWrite(green_led, LOW);
    }
    get_time();
    if ((sts_mode == 0) && (mains_input_value > 50)) {
      gsm_func();
    }
  }

  if(Mode_select == 2)
  {
    //OpenPayGo Token code;
  }
}
