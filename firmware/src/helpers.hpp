#pragma once

// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "credit.hpp"
#include "lcd_display.hpp"
#include "power.hpp"
#include "sts_token.hpp"
#include "token_management.hpp"

float pulsetime_now = 0.0;
float prev_pulsetime = 0.0;

byte token_ok = 0;

float btValue = 0.0;

byte fault_written = 0;

unsigned int over_voltage = 250;
unsigned int over_load = 25000;
unsigned int low_voltage = 150;
unsigned int low_freq = 45;

void urgeent() {
  if (true_power < 15) {
    true_power = 0;
  }

  pulsetime_now = millis();
  if (pulsetime_now - prev_pulsetime > 1000) {
    prev_pulsetime = pulsetime_now;
    credit_formular();
  }

  STS_keypad();  // scan to see if a key is pressed.

  warn_now = millis();
  if (c_chek == 1) {
    if (warn_now - warntime > 1500) {
      sts_data = "";
      sts_mode = 0;
      data_count = 0;
      sts_value = 0;
      sts_accept = 0;
      topupnew = 0;
      topup = 0;
      c_chek = 0;
      token_ok = 0;
      token_used = 0;
      warntime = warn_now;
      digitalWrite(buzzer, LOW);
    }
  }

  if (parameters > 0) {
    parameters_display();
  }

  if ((sts_mode == 0) && (parameters == 0) && (btt > 2) && (tp_fetch == 0)) {
    lcd_update();
  }

  // btValue = analogRead(battery_input);
  // btt = btValue * 0.07331280915044247787610619469027;
  // if (btt < 2) {
  //   fault = 1;
  //   if (fault_written == 0) {
  //     mem.writeLong(tamper_location, 03);
  //     fault_written = 1;
  //   }
  //   lcd.setCursor(0, 0);
  //   lcd.print("TERMINAL  COVER ");
  //   lcd.setCursor(0, 1);
  //   lcd.print("      OPEN      ");
  //   digitalWrite(buzzer, HIGH);
  //   delay(20);
  //   digitalWrite(buzzer, LOW);
  // }

  if (btt > 2) {
    fault = 0;
    fault_written = 0;
  }

  if (mains_input_value > 50) {
    if ((freq < low_freq) || (mains_input_value > over_voltage) ||
        (mains_input_value < low_voltage) || (true_power > over_load)) {
      fault = 1;
    }
    if (((freq > low_freq) || (mains_input_value < over_voltage) ||
         (mains_input_value > low_voltage) || (true_power < over_load)) &&
        (btt > 2)) {
      fault = 0;
    }
  }
}

uint32_t convertArrayToUint32(int arrayToConvert[9]) {
  int i = 0;
  uint32_t uint32 = 0;
  for (i = 0; i < 9; i++) {
    uint32 = 10 * uint32 + arrayToConvert[i];
  }
  return (uint32);
}

uint32_t convertByteArrayToUint32(uint8_t arrayToConvert[4]) {
  uint32_t low1 = arrayToConvert[0];
  uint32_t low2 = arrayToConvert[1];
  low2 = low2 << 8;
  uint32_t high1 = arrayToConvert[2];
  high1 = high1 << 16;
  uint32_t high2 = arrayToConvert[3];
  high2 = high2 << 24;
  uint32_t result = low1 + low2 + high1 + high2;
  return (result);
}

void convertUint32ToUint8Array(uint32_t uint32ToConvert,
                               uint8_t arrayBytes[4]) {
  byte low1 = uint32ToConvert;
  byte low2 = uint32ToConvert >> 8;
  byte high1 = uint32ToConvert >> 16;
  byte high2 = uint32ToConvert >> 24;
  arrayBytes[0] = low1;
  arrayBytes[1] = low2;
  arrayBytes[2] = high1;
  arrayBytes[3] = high2;
}

void storeUint32InNvram(uint32_t toStore, int address) {
  uint8_t arrayBytes[4];
  convertUint32ToUint8Array(toStore, arrayBytes);
  rtc.writenvram(address, arrayBytes, 4);
}

uint32_t readUint32FromNvram(int address) {
  uint8_t readData[4] = {0};
  rtc.readnvram(readData, 4, address);
  return (convertByteArrayToUint32(readData));
}
