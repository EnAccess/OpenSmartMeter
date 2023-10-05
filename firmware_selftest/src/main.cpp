// defines
#define RTC_I2C_ADDR 0X68
#define EEPROM_I2C_ADDR 0X50

#define MODEM_SERIAL Serial1
#define TINY_GSM_DEBUG Serial2

#define BUZZER PB5
#define BUZZER_FREQ 500  // For Passive Buzzer
#define BUZZER_DURATION 2000

#define RELAY_A PB12
#define RELAY_B PB4

#define RED_LED PA0
#define GREEN_LED PA4

// Modem Type be defined before library inclusion
#define TINY_GSM_MODEM_SIM800

// Arduino base libraries
#include <Arduino.h>
#include <Wire.h>

// third party libraries
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <TinyGsmClient.h>

// Serial for CLI
HardwareSerial Serial2(PA3, PA2);

char daysOfTheWeek[7][12] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                             "Thursday", "Friday", "Saturday"};

// LCD Pins Setup
const int rs = PB3, en = PA15, d4 = PA8, d5 = PB15, d6 = PB14, d7 = PB13;

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {PB9, PB8, PB1, PB0};
byte colPins[COLS] = {PA7, PA6, PA5, PC13};

// LCD Object Instance
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Keypad Object Instance
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// TinyGsm Object Instance
TinyGsm modem(MODEM_SERIAL);

void relay_on() {
  digitalWrite(RELAY_A, HIGH);
  digitalWrite(RELAY_B, LOW);
  delay(200);
  digitalWrite(RELAY_A, LOW);
  digitalWrite(RELAY_B, LOW);
}

void relay_off() {
  digitalWrite(RELAY_A, LOW);
  digitalWrite(RELAY_B, HIGH);
  delay(200);
  digitalWrite(RELAY_A, LOW);
  digitalWrite(RELAY_B, LOW);
}

int rtc_test() {
  Serial2.println("Checking for RTC");
  Serial2.print("RTC I2C Address is: ");
  Serial2.println(RTC_I2C_ADDR);

  Wire.begin();
  Wire.beginTransmission(RTC_I2C_ADDR);
  int result = Wire.endTransmission();
  if (result == 0) {
    Serial2.println("RTC FOUND!");
  } else {
    Serial2.println("RTC NOT FOUND");
  }

  return result;
}

int eeprom_test() {
  Serial2.println("Checking for EEPROM");
  Serial2.print("EEPROM I2C Address is: ");
  Serial2.println(EEPROM_I2C_ADDR);

  Wire.begin();
  Wire.beginTransmission(EEPROM_I2C_ADDR);
  int result = Wire.endTransmission();
  if (result == 0) {
    Serial2.println("EEPROM FOUND!");
  } else {
    Serial2.println("EEPROM NOT FOUND");
  }
  return result;
}

uint32_t modem_test() {
  // Module baud rate
  uint32_t rate = 0;  // Set to 0 for Auto-Detect

  if (!rate) {
    rate = TinyGsmAutoBaud(MODEM_SERIAL);
  }

  if (!rate) {
    Serial2.println(
        F("***********************************************************"));
    Serial2.println(F(" Modem Test Failed!"));
    Serial2.println(
        F("***********************************************************"));
  } else {
    Serial2.println(
        F("***********************************************************"));
    Serial2.println(F(" Modem Found"));
    Serial2.println(
        F("***********************************************************"));
  }

  return rate;
}

void setup() {
  uint32_t result = 0xFF;
  // put your setup code here, to run once:
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(RELAY_A, OUTPUT);
  pinMode(RELAY_B, OUTPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(LED_BUILTIN,
               HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);         // wait for a second
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  delay(1000);                     // wait for a second

  Serial2.begin(115200);
  lcd.begin(16, 2);

  Serial2.println("Hardware Self Test");

  lcd.setCursor(0, 0);
  lcd.print("    HARDWARE    ");
  lcd.setCursor(0, 1);
  lcd.print("   SELF-TEST    ");
  delay(5000);
  lcd.clear();

  // Test Buzzer
  Serial2.println("Buzzer Test");
  lcd.setCursor(0, 0);
  lcd.print("  BUZZER TEST  ");
  tone(BUZZER, BUZZER_FREQ, BUZZER_DURATION);
  delay(BUZZER_DURATION);
  lcd.clear();

  // Test Relay
  Serial2.println("Relay Test");
  lcd.setCursor(0, 0);
  lcd.print("  RELAY TEST  ");
  relay_on();
  delay(2000);
  relay_off();
  delay(2000);
  lcd.clear();

  // Test LEDs
  Serial2.println("LEDs Test");
  lcd.setCursor(0, 0);
  lcd.print("  LEDs TEST  ");
  // RED
  digitalWrite(RED_LED, HIGH);
  delay(1000);
  digitalWrite(RED_LED, LOW);
  // GREEN
  digitalWrite(GREEN_LED, HIGH);
  delay(1000);
  digitalWrite(GREEN_LED, LOW);
  lcd.clear();

  // Test EEPROM
  Serial2.println("EEPROM Test");
  lcd.setCursor(0, 0);
  lcd.print("  EEPROM TEST  ");
  result = eeprom_test();
  lcd.setCursor(0, 1);
  if (0 == result) {
    lcd.print("  EEPROM FOUND  ");
  } else {
    lcd.print(" EEPROM NOT FOUND ");
  }
  delay(5000);
  lcd.clear();

  // Test RTC
  Serial2.println("RTC Test");
  lcd.setCursor(0, 0);
  lcd.print("  RTC TEST  ");
  result = 0xFF;
  result = rtc_test();
  lcd.setCursor(0, 1);
  if (0 == result) {
    lcd.print("  RTC FOUND  ");
  } else {
    lcd.print(" RTC NOT FOUND ");
  }
  delay(5000);
  lcd.clear();

  // Test Modem
  Serial2.println("Modem Test");
  lcd.setCursor(0, 0);
  lcd.print("  MODEM TEST  ");
  result = 0xFF;
  result = modem_test();
  lcd.setCursor(0, 1);
  if (0 == result) {
    lcd.print(" MODEM NOT FOUND ");
  } else {
    lcd.print(" MODEM FOUND ");
  }
  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("  PRESS KEYPAD  ");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial2.available()) {        // If anything comes in,
    Serial2.write(Serial2.read());  // read it and send it out
  }

  char key = customKeypad.getKey();

  if (key) {
    Serial2.println(key);
    lcd.setCursor(0, 1);
    lcd.print(key);
  }
}
