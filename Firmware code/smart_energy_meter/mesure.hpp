#pragma once
#include "global_defines.hpp"

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