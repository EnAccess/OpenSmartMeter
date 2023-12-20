#pragma once

// defines

// Arduino base libraries

// third party libraries
#include <RTClib.h>

// OpenSmartMeter libraries
#include "mem_init.hpp"
#include "mem_operation.hpp"
#include "thingsboard.hpp"

#define TIME_DIVIDER 1
#define ACTIVE_UNTIL_NVRAM_ADDRESS \
  0  // 56 bytes address from 0 to 55 in the NVRAM
#define TIME_INITIALIZATION_NVRAM_ADDRESS 4

RTC_DS1307 rtc;
uint32_t timeInitializationRtc = 0;

unsigned int hours, minutes, seconds, rtcday = 0;
long nw_month_cnt, rtcmonth, rtcnewmonth, billing_date = 0;

byte thingsboard_check = 0;

float lastmonth_KWH = 0.0;

// For STS

void get_time() {
  DateTime now = rtc.now();
  hours = (now.hour());
  minutes = (now.minute());
  seconds = (now.second());
  rtcday = (now.day());
  rtcmonth = (now.month(), DEC);
  if (nw_month_cnt == 0) {
    rtcnewmonth = rtcmonth;
    nw_month_cnt = 1;
  }
  if (minutes == 30) {
    mem.writeLong(credit_eeprom_location, creditt);
    delay(10);  // update newly added topup value //
  }

  if ((hours == 23) && (thingsboard_check == 0)) {
    post_to_thingsboard();
    thingsboard_check = 1;
  }

  if ((hours < 23) || (hours > 23)) {
    thingsboard_check = 0;
  }

  if ((rtcmonth > rtcnewmonth) && (nw_month_cnt == 1)) {
    lastmonth_KWH = ENERGY;
    billing_date = rtcday;
    nw_month_cnt = 0;
  }
}

// For Opaygo

uint32_t GetTimeInSeconds() {
  /*
   *Returns the unixtime in seconds of Now
   */
  DateTime now = rtc.now();
  uint32_t nowInSeconds =
      (now.unixtime() -
       timeInitializationRtc);  // we substract the init time so that it's
                                // easier to read when debugging, and works the
                                // same as Arduino Time mgt
  return (nowInSeconds);
}

void AddTime(int ActivationValue) {
  uint32_t Now = GetTimeInSeconds();
  int NumberOfSecondsToActivate = (ActivationValue * 3600 * 24) / TIME_DIVIDER;

  if (ActiveUntil < Now) {
    ActiveUntil = Now;
  }

  ActiveUntil +=
      NumberOfSecondsToActivate;  // We add the number of days (converted in
                                  // seconds for to compare to our RTC time)
}

void SetTime(int ActivationValue) {
  uint32_t Now = GetTimeInSeconds();
  int NumberOfSecondsToActivate = (ActivationValue * 3600 * 24) / TIME_DIVIDER;

  ActiveUntil =
      Now +
      NumberOfSecondsToActivate;  // We set the number of days (converted in
                                  // seconds for to compare to our RTC time)
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

void initializeTime() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  // We check that it is not a disconnection power - Arduino
  uint32_t nvramCheck = readUint32FromNvram(TIME_INITIALIZATION_NVRAM_ADDRESS);
  if (nvramCheck == 0) {  // it is the first setup of the Arduino
    rtc.adjust(
        DateTime(F(__DATE__), F(__TIME__)));  // sets the RTC to the date & time
                                              // this sketch was compiled
    DateTime now = rtc.now();
    timeInitializationRtc = now.unixtime();
    storeUint32InNvram(timeInitializationRtc,
                       TIME_INITIALIZATION_NVRAM_ADDRESS);
  } else {  // a disconnection happened
    timeInitializationRtc = nvramCheck;
    incrementNbDisconnectionsEeprom();
    storeTimeStampEEPROM(
        GetTimeInSeconds());  // to avoid counting twice the disconnection if
                              // the previous TimeStamp was too long ago
  }
}
