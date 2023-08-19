#ifndef power_hpp
#define power_hpp

// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "SAM_UART.h"

float true_power = 0.0;
float ENERGY = 0.0;
float freq = 0.0;
float curr = 0.0;
float powerFactor = 0.0;
float mains_input_value = 0.0;

HardwareSerial ATM90E26(PB11, PB10);
ATM90E26_UART AFE_chip(&ATM90E26);

void mesure() {
  freq = AFE_chip.FETCH_Frequency();
  delay(20);
  powerFactor = AFE_chip.FETCH_PowerFactor();
  delay(20);
  mains_input_value = AFE_chip.FETCH_mains();
  delay(20);
  curr = AFE_chip.FETCH_NEUTRALCurrent();
  curr = curr * 2;
  delay(20);
  true_power = curr * mains_input_value * powerFactor;
  if (true_power < 5) {
    curr = 0;
  }
  delay(20);
}

#endif