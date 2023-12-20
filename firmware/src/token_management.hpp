#pragma once

// defines
#define Lengths 20

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "lcd_init.hpp"
#include "mem_init.hpp"
#include "sts_token.hpp"

char Data[Lengths];
int lcd_count = 0;
byte data_count = 0;
byte dt = 0;
byte parameters = 0;
byte token_used = 0;
String password = "1234";

//    for meter
unsigned long sts_eeprom_fetched = 0;
unsigned long token_eeprom_location = 20;
unsigned long eeprom_location_cnt = 40;

unsigned long sts_value = 0;
unsigned long sts_mode = 0;
unsigned long Mode_select = 0;

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
