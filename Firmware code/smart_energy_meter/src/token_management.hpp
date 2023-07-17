#pragma once

// defines
#define Lengths 20

// Arduino base libraries

// third party libraries
#include <Keypad.h>

// OpenSmartMeter libraries
#include "lcd_init.hpp"
#include "mem_init.hpp"
#include "sts_token.hpp"

char customKey;

// keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {PB9, PB8, PB1, PB0};
byte colPins[COLS] = {PA7, PA6, PA5, PC13};
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char Data[Lengths];

int lcd_count = 0;
byte data_count = 0;
byte dt = 0;
byte parameters = 0;
byte token_used = 0;

//    for meter
unsigned long sts_eeprom_fetched = 0;
unsigned long token_eeprom_location = 20;
unsigned long eeprom_location_cnt = 40;

unsigned long sts_value = 0;
unsigned long sts_mode = 0;
unsigned long Mode_select = 0;

void buss() {
  digitalWrite(buzzer, HIGH);
  delay(10);
  digitalWrite(buzzer, LOW);
}

void check_tokenused() {
  token_eeprom_location = mem.readLong(eeprom_location_cnt);
  if (token_eeprom_location > 4000) {
    mem.writeLong(eeprom_location_cnt, 40);
    token_eeprom_location = 40;
  }
  // token_eeprom_location = token_eeprom_location +1;
  unsigned int locationcount;
  for (locationcount = 40; locationcount < token_eeprom_location;
       locationcount++) {
    sts_eeprom_fetched = mem.readLong(locationcount);
    String ee_fetched = String(sts_eeprom_fetched);
    String sts_keyco = sts_data.substring(0, 10);
    String keyco = ee_fetched.substring(0, 10);

    long conv_keyco = keyco.toInt();
    long conv_sts_keyco = sts_keyco.toInt();
    if ((conv_keyco == conv_sts_keyco)) {
      if (c_chek == 0) {
        warntime = warn_now;
        c_chek = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("  WARNING!!!!!  ");
      lcd.setCursor(0, 1);
      lcd.print("   USED TOKEN   ");
      digitalWrite(buzzer, HIGH);
      token_used = 1;
    }
  }
}

void STS_keypad() {
  customKey = customKeypad.getKey();
  if (customKey == '*') {
    data_count = 0;
    lcd_count = 12;
    dt = 0;
    sts_value = 0;
    delay(20);
    buss();
    lcd.clear();
    lcd.setCursor(0, 0);
    sts_mode = 1;
    if (customKeypad.getState() == HOLD)
    { 
      lcd.println("Password: ");
    }
    else{
      lcd.print("TOKEN: ");
    }
    
  }

  if (sts_mode == 1 && customKey != '*' && customKey != '#' &&
      customKey != 'D') {
    if (customKey) {
      if (dt < 21) {
        delay(20);
        buss();
        dt++;
        if (dt < 5) {
          Data[data_count] = customKey;
          lcd.setCursor(lcd_count, 0);
          lcd.print(Data[data_count]);
          data_count++;
        }
        if (lcd_count > 15) {
          lcd_count = 0;
        }
        if (dt > 4) {
          Data[data_count] = customKey;
          lcd.setCursor(lcd_count, 1);
          lcd.print(Data[data_count]);
          data_count++;
        }
        lcd_count++;
        sts_value = data_count;
      }
    }
  }
  if (customKey == 'D') {
    sts_data = "";
    sts_mode = 0;
    sts_value = 0;
  }

  if (customKey == 'A' && sts_mode == 0) {
    parameters = parameters + 1;
  }

  if (customKey == '#' && sts_mode == 1) {
    sts_data = Data;
    if(data_count > 19)
    {
      check_tokenused();
      if (token_used == 0) 
      {
        STStoken_decode();
      }
    }

    if(data_count < 19 &&  data_count > 3)
    {
      if (sts_data == password)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("MODE: ");
        data_count = 0;
        lcd_count = 8;
        dt = 0;
      }
      if (sts_data == "112")
      {  
        Mode_select = 1;
      }

      if (sts_data == "122")
      {
          Mode_select = 2;
      }
    }
  }
}