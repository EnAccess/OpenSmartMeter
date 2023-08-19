#ifndef lcd_init_hpp
#define lcd_init_hpp

// defines

// Arduino base libraries

// third party libraries
#include <LiquidCrystal.h>

// OpenSmartMeter libraries

const int rs = PB3, en = PA15, d4 = PA8, d5 = PB15, d6 = PB14, d7 = PB13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#endif