#pragma once

// defines

// Arduino base libraries

// third party libraries
#include <RTClib.h>

// OpenSmartMeter libraries
#include "mem_init.hpp"
#include "thingsboard.hpp"
#include "helpers.hpp"

RTC_DS1307 rtc;
#define ACTIVE_UNTIL_NVRAM_ADDRESS 0 // 56 bytes address from 0 to 55 in the NVRAM
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


uint32_t convertByteArrayToUint32(uint8_t arrayToConvert[4]){
  uint32_t low1 = arrayToConvert[0];
  uint32_t low2 = arrayToConvert[1];
  low2 = low2 << 8;
  uint32_t high1 = arrayToConvert[2];
  high1 = high1 << 16;
  uint32_t high2 = arrayToConvert[3];
  high2 = high2 << 24;
  uint32_t result = low1 + low2 + high1 + high2;
  return(result);
}

void convertUint32ToUint8Array (uint32_t uint32ToConvert, uint8_t arrayBytes[4])
{
  byte low1 = uint32ToConvert;
  byte low2 = uint32ToConvert >> 8;
  byte high1 = uint32ToConvert >> 16;
  byte high2 = uint32ToConvert >> 24;
  arrayBytes[0] = low1;
  arrayBytes[1] = low2;
  arrayBytes[2] = high1;
  arrayBytes[3] = high2;
}

void storeUint32InNvram(uint32_t toStore, int address){
    uint8_t arrayBytes[4];
    convertUint32ToUint8Array(toStore, arrayBytes);
    rtc.writenvram(address, arrayBytes, 4);
  }


  uint32_t readUint32FromNvram(int address){
    uint8_t readData[4] = {0};
    rtc.readnvram(readData, 4, address);
    return(convertByteArrayToUint32(readData));
  }


void initializeTime(){
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
    while (1);
    }
    
    if (! rtc.isrunning()) {
      Serial.println("RTC is NOT running!");
    }

    //We check that it is not a disconnection power - Arduino
    uint32_t nvramCheck = readUint32FromNvram(TIME_INITIALIZATION_NVRAM_ADDRESS);
    if (nvramCheck == 0){ // it is the first setup of the Arduino
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //sets the RTC to the date & time this sketch was compiled
      DateTime now = rtc.now();
      timeInitializationRtc = now.unixtime();
      storeUint32InNvram(timeInitializationRtc, TIME_INITIALIZATION_NVRAM_ADDRESS);
    } 
    else{ // a disconnection happened
      timeInitializationRtc = nvramCheck;
      //incrementNbDisconnectionsEeprom(); //work on this after
      //storeTimeStampEEPROM(getTimeInSeconds()); // to avoid counting twice the disconnection if the previous TimeStamp was too long ago
    }
  }