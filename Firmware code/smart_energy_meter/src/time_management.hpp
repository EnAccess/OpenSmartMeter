#ifndef time_management_hpp
#define time_management_hpp
// #pragma once

// defines

// Arduino base libraries

// third party libraries
#include <RTClib.h>

// OpenSmartMeter libraries
#include "mem_init.hpp"
#include "thingsboard.hpp"

RTC_DS1307 rtc;
#define ACTIVE_UNTIL_NVRAM_ADDRESS \
  0  // 56 bytes address from 0 to 55 in the NVRAM
#define TIME_INITIALIZATION_NVRAM_ADDRESS 4
uint32_t timeInitializationRtc = 0;

unsigned int hours, minutes, seconds, rtcday = 0;
long nw_month_cnt, rtcmonth, rtcnewmonth, billing_date = 0;

byte thingsboard_check = 0;

float lastmonth_KWH = 0.0;

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

#endif
