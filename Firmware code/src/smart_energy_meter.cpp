#include "smart_energy_meter.h"
#include "global_defines.h"
#include "time_management.hpp"
#include "remote.hpp"
#include "credit.hpp"
#include "mesure.hpp"


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