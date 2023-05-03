// defines

// Arduino base libraries
#include <Wire.h>
#include "Arduino.h"

// third party libraries
#include <ArduinoHttpClient.h>
#include <LiquidCrystal.h>

// OpenSmartMeter libraries
#include "credit.hpp"
#include "global_defines.hpp"
#include "lcd_init.hpp"
#include "mem_init.hpp"
#include "power.hpp"
#include "relay.hpp"
#include "remote.hpp"
#include "sts_token.hpp"
#include "thingsboard.hpp"
#include "time_management.hpp"
#include "token_management.hpp"

HardwareSerial Serial2(PA3, PA2);

byte data_count2, encoder = 0;

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";

unsigned int over_voltage = 250;
unsigned int over_load = 25000;
unsigned int low_voltage = 150;
unsigned int low_freq = 45;

String sts_data1 = "";

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

unsigned long eepromupdate_time, prev_energypulse, new_energypulse = 0;
float deduction_factor = 0.0;
float lcd_creditt, totalbill, energy_billing, ENERGY2, btValue = 0.0;
float prevpulsecounttime, pulsetiming, pulsecounttime = 0.0;
unsigned long LCD_scroll_time =
    5000;  // 10sec according to Nigeria metering code
unsigned long lcdtime_now, prev_lcdtime = 0;
unsigned long lcd_refresh, lcd_reset, mains_inputV, cnt, p_time = 0;
unsigned long set_inv_wattage, pv_v, u, mains_v, j, h, m = 0;
float cu, dc, t_cu, btt, currinv_adc, currmains_adc, p_mains, curr_mains_Value,
    curr_inv_Value, init_curr, true_power2, currnew = 0.0;
float pvvalue, mainsvalue, curinv_v, curnep_v, relcur, but, bat_curr,
    bat_curr_value, p_inv, inv_power, mains_power = 0.000;
byte internetpost_time, internetget_time, internetenergy_time, credit_time,
    sucess = 0;
unsigned int pulse, pulse_set = 0;
unsigned int pulse_delay = 200;
unsigned long pulsetime, current_time, previous, previousenergytime,
    previousenergytime2, energytime, energytime2, currentenergytime,
    currentenergytime2 = 0;
byte fault_written = 0;
int tamper_log = 0;

float pulsetime_now, prev_pulsetime = 0.0;
int tamper_location = 5;
byte token_ok = 0;

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
  MyTim->attachInterrupt(urgeent);
  MyTim->resume();
}

void loop() {
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
