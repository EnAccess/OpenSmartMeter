HardwareSerial Serial2(PA3, PA2);
#include <SAM_UART.h>
HardwareSerial ATM90E26(PB11, PB10);
ATM90E26_UART AFE_chip(&ATM90E26);

// keypad
#include <Keypad.h>
#define Lengths 20
char Data[Lengths];
int lcd_count, convertedsts_data = 0;
byte data_count, data_count2, dt, sts_accept, encoder = 0;
char customKey;
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {PB9, PB8, PB1, PB0};
byte colPins[COLS] = {PA7, PA6, PA5, PC13};
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 60  // Set RX buffer to 1Kb
#include <ArduinoHttpClient.h>
#include <TinyGsmClient.h>
#include "ThingsBoard.h"

const char apn[] = "terminal";
const char user[] = "";
const char pass[] = "";

#define TOKEN "5GBw6kqNCN93BN3nuuvJ"  //"YOUR_ACCESS_TOKEN"
#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define THINGSBOARD_PORT 80

TinyGsm modem(Serial1);
TinyGsmClient client(modem);
ThingsBoard tb(client);
bool modemConnected = false;

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";

const char server[] =
    "paygotesting.000webhostapp.com";  // domain name: example.com,
                                       // maker.ifttt.com, etc
const char resource[] =
    "/subscribe.php";  // resource path, for example: /post-data.php
const int port = 80;   // server port number
HttpClient http(client, server, port);
String apiKeyValue = "tPmAT5Ab3j7F9";

unsigned int over_voltage = 250;
unsigned int over_load = 25000;
unsigned int low_voltage = 150;
unsigned int low_freq = 45;

unsigned int tariff = 70;
String stat = "sucess";
String sts_data = "";
String sts_data1 = "";
int private_stskey = 109;

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
unsigned int hours, minutes, seconds, rtcday = 0;
#include <AT24CX.h>
// EEPROM object
AT24CX mem;

#include <LiquidCrystal.h>
const int rs = PB3, en = PA15, d4 = PA8, d5 = PB15, d6 = PB14, d7 = PB13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define buzzer PB5
#define relaya PB12
#define relayb PB4

#define red_led PA0
#define green_led PA4
#define battery_input PA1

unsigned int encodernew = 0;
unsigned long eepromupdate_time, eeprom_sts_data, prev_energypulse,
    new_energypulse, topupnew = 0;
float deduction_factor, topup, creditt = 0.0;
float lcd_creditt, totalbill, energy_billing, dcode, ENERGY, ENERGY2,
    powerFactor, freq, btValue = 0.0;
float prevpulsecounttime, pulsetiming, pulsecounttime = 0.0;
unsigned long LCD_scroll_time =
    5000;  // 10sec according to Nigeria metering code
unsigned long lcdtime_now, prev_lcdtime = 0;
unsigned long lcd_refresh, lcd_reset, mains_inputV, cnt, p_time = 0;
unsigned long set_inv_wattage, pv_v, u, mains_v, j, h, m, sts_mode = 0;
float cu, dc, t_cu, btt, currinv_adc, currmains_adc, p_mains, curr_mains_Value,
    curr_inv_Value, init_curr, billing, curr, true_power2, currnew = 0.0;
float pvvalue, mainsvalue, curinv_v, curnep_v, relcur, but, bat_curr,
    bat_curr_value, p_inv, inv_power, mains_power = 0.000;
byte internetpost_time, internetget_time, internetenergy_time, credit_time,
    sucess, parameters = 0;
unsigned int pulse, pulse_set = 0;
unsigned int pulse_delay = 200;
unsigned long sts_value, convertedsts_day, pulsetime, current_time, previous,
    previousenergytime, previousenergytime2, energytime, energytime2,
    currentenergytime, currentenergytime2 = 0;
byte get_credit, fault, fault_written = 0;
byte off_exec = 0;
byte on_exec = 0;
int confirmkey, tamper_log = 0;
byte token_used, thingsboard_check = 0;

float true_power, mains_input_value, lastmonth_KWH = 0.0;
float pulsetime_now, prev_pulsetime = 0.0;
long warn_now = 0;
int tamper_location = 5;
long warntime, nw_month_cnt, rtcmonth, rtcnewmonth, billing_date = 0;
byte c_chek, tp_fetch, token_ok = 0;

unsigned long token_eeprom_location = 20;
unsigned long eeprom_location_cnt = 40;
unsigned long credit_eeprom_location = 9;

//    for meter
String meter = "MT100";
unsigned long meter_no = 100;
unsigned long multiplier = 100;  // to raise meter no to 5 digit
unsigned int meter_no_count = 3;
unsigned long sts_eeprom_fetched = 0;

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

void urgeent() {
  if (true_power < 15) {
    true_power = 0;
  }

  pulsetime_now = millis();
  if (pulsetime_now - prev_pulsetime > 1000) {
    prev_pulsetime = pulsetime_now;
    credit_formular();
  }

  STS_keypad();
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

  btValue = analogRead(battery_input);
  btt = btValue * 0.07331280915044247787610619469027;
  if (btt < 2) {
    fault = 1;
    if (fault_written == 0) {
      mem.writeLong(tamper_location, 03);
      fault_written = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("TERMINAL  COVER ");
    lcd.setCursor(0, 1);
    lcd.print("      OPEN      ");
    digitalWrite(buzzer, HIGH);
    delay(20);
    digitalWrite(buzzer, LOW);
  }
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

void relay_on() {
  if (on_exec == 0) {
    digitalWrite(relaya, HIGH);
    digitalWrite(relayb, LOW);
    delay(200);
    digitalWrite(relaya, LOW);
    digitalWrite(relayb, LOW);
    on_exec = 1;
    off_exec = 0;
  }
}

void relay_off() {
  if (off_exec == 0) {
    digitalWrite(relaya, LOW);
    digitalWrite(relayb, HIGH);
    delay(200);
    digitalWrite(relaya, LOW);
    digitalWrite(relayb, LOW);
    off_exec = 1;
    on_exec = 0;
  }
}

void lcd_update() {
  lcdtime_now = millis();
  if (lcdtime_now - prev_lcdtime >= LCD_scroll_time) {
    cnt = cnt + 1;
    prev_lcdtime = lcdtime_now;
    lcd.begin(16, 2);
  }

  // scene 1
  if (cnt == 0) {
    lcd.setCursor(0, 0);
    lcd.print("V=     ");
    lcd.setCursor(2, 0);
    lcd.print(mains_input_value);
    lcd.setCursor(7, 0);
    lcd.print(" ");

    lcd.setCursor(8, 0);
    lcd.print("C=     ");
    lcd.setCursor(10, 0);
    lcd.print(curr);
    lcd.setCursor(15, 0);
    lcd.print("A");

    lcd.setCursor(0, 1);
    lcd.print("W=          ");
    lcd.setCursor(2, 1);
    lcd.print(true_power);
    lcd.setCursor(8, 1);
    lcd.print(" ");
    lcd.setCursor(9, 1);
    lcd.print("PF=        ");
    lcd.setCursor(12, 1);
    lcd.print(powerFactor);
  }

  // scene 2
  if (cnt == 1) {
    lcd.setCursor(0, 0);
    lcd.print(" CREDIT=#       ");
    lcd.setCursor(9, 0);
    lcd.print(creditt);

    lcd.setCursor(0, 1);
    lcd.print("ENERGY=        ");
    lcd.setCursor(7, 1);
    lcd.print(ENERGY);
    lcd.setCursor(12, 1);
    lcd.print("KW/H");
  }
  if (cnt > 1) {
    cnt = 0;
  }
}

void credit_formular() {
  if (true_power > 0) {
    ENERGY = ENERGY + ((true_power) /
                       (3600000));  // energy per KWH(energy gotten per sec)
    billing = (true_power * tariff) / 3600000;  // cost per sec
    if (creditt > billing) {
      creditt = (creditt - billing);
    }
  }
}

void credit_reminder() {
  if (creditt > 1 && creditt < 200) {
    digitalWrite(red_led, HIGH);
    digitalWrite(green_led, LOW);
    delay(80);
    digitalWrite(red_led, LOW);
    delay(80);
  }
  if (creditt < 1 || fault == 1) {
    digitalWrite(red_led, HIGH);
    relay_off();
  }
  if (creditt > 200 && fault == 0) {
    digitalWrite(red_led, LOW);
    relay_on();
    digitalWrite(green_led, HIGH);
  }
}

void parameters_display() {
  if (parameters == 1) {
    lcd.setCursor(0, 0);
    lcd.print("  Meter number  ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(3, 1);
    lcd.print(meter);
  }
  if (parameters == 2) {
    lcd.setCursor(0, 0);
    lcd.print("date/time :     ");
    lcd.setCursor(0, 1);
    lcd.print("                ");

    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(rtcday);
    lcd.setCursor(3, 1);
    lcd.print("/");

    lcd.setCursor(4, 1);
    lcd.print(hours);
    lcd.setCursor(6, 1);
    lcd.print(":");

    lcd.setCursor(7, 1);
    lcd.print(minutes);
    lcd.setCursor(9, 1);
    lcd.print(":");

    lcd.setCursor(10, 1);
    lcd.print(seconds);
    lcd.setCursor(12, 1);
    lcd.print("     ");
  }
  if (parameters == 3) {
    lcd.setCursor(0, 0);
    lcd.print("battery level :  ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(6, 1);
    lcd.print(btt);
  }
  if (parameters == 4) {
    lcd.setCursor(0, 0);
    lcd.print("TARIFF =        ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(6, 1);
    lcd.print(tariff);
  }
  if (parameters == 5) {
    lcd.setCursor(0, 0);
    lcd.print("Fault event :   ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    tamper_log = mem.readLong(tamper_location);
    lcd.setCursor(6, 1);
    lcd.print(tamper_log);
  }

  if (parameters == 6) {
    lcd.setCursor(0, 0);
    lcd.print("Last month      ");
    lcd.setCursor(0, 1);
    lcd.print("billing date :  ");
    lcd.setCursor(14, 1);
    lcd.print(lastmonth_KWH);
  }
  if (parameters == 7) {
    lcd.setCursor(0, 0);
    lcd.print(" Last month KWH ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(6, 1);
    lcd.print(lastmonth_KWH);
    lcd.setCursor(12, 1);
    lcd.print("KW/H");
  }
  if (parameters > 7) {
    parameters = 0;
  }
}

void gsm_func() {
  Serial2.print("Connecting to APN: ");
  Serial2.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial2.println(" fail");
  } else {
    Serial2.println(" OK");
    Serial2.print("Connecting to ");
    Serial2.print(server);
  }
  if (!client.connect(server, port)) {
    Serial2.println(" fail");
  }

  else {
    Serial2.println(" OK");
    Serial2.println("Performing HTTP POST request...");
    String httpRequestData =
        "api_key=" + apiKeyValue + "&meter=" + String(meter) + "";

    client.print(String("POST ") + resource + " HTTP/1.1\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(httpRequestData.length());
    client.println();
    client.println(httpRequestData);

    String s_received_credit, s_tariff, readString;
    byte ind1;
    byte ind2;
    String body = http.responseBody();
    readString = body;
    ind1 = readString.indexOf("/");  // finds location of first
    s_received_credit =
        readString.substring(0, ind1);         // cpture received credit
    ind2 = readString.indexOf("/", ind1 + 1);  // finds location of second,
    s_tariff = readString.substring(ind1 + 1,
                                    ind2 + 1);  // captures second data String

    Serial2.println(F("topup:"));
    topup = s_received_credit.toFloat();
    Serial2.println(topup);
    tariff = s_tariff.toInt();
    http.stop();
    client.stop();
    Serial2.println(F("Server disconnected"));

    if (topup > 0) {  // if topup available //
      post_sucess();  // update sucess //
      tp_fetch = 1;
      lcd.setCursor(0, 0);
      lcd.print(" TOKEN RECEIVED ");
      lcd.setCursor(0, 1);
      lcd.print("     VIA WEB    ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print(" TOPUP CASH :   ");
      lcd.setCursor(0, 1);
      lcd.print(topup);
      // add topup to eeprom //
      mem.writeLong(credit_eeprom_location, creditt);
      creditt =
          mem.readLong(credit_eeprom_location);  // fetch previous credit //
      creditt = creditt + topup;  // add new   if any to old credit //
      mem.writeLong(credit_eeprom_location,
                    creditt);  // update newly added topup value //
      delay(1500);
      tp_fetch = 0;
    }
    topup = 0;  // reset topup value back to 0//
  }

  Serial2.print("POWER FACTOR : ");
  Serial2.println(powerFactor);
  Serial2.print("FREQUENCY : ");
  Serial2.println(freq);
  Serial2.print("MAINS VOLTAGE : ");
  Serial2.println(mains_input_value);
  Serial2.print("CURRENT  : ");
  Serial2.println(curr);
  Serial2.print("ACTIVE POWER : ");
  Serial2.println(true_power);
  Serial2.print("ENERGY : ");
  Serial2.println(ENERGY);
  Serial2.print("CREDIT : ");
  Serial2.println(creditt);
}

void post_sucess() {
  Serial2.print("Connecting to APN: ");
  Serial2.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial2.println(" fail");
  }

  else {
    Serial2.println(" OK");
    Serial2.print("Connecting to ");
    Serial2.print(server);
  }
  if (!client.connect(server, port)) {
    Serial2.println(" fail");
  } else {
    Serial2.println(" OK");
  }

  Serial2.println("Performing HTTP POST request...");
  String httpRequestData = "api_key=" + apiKeyValue + "&stat=" + String(stat) +
                           "&meter=" + String(meter) + "";

  client.print(String("POST ") + resource + " HTTP/1.1\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.println("Connection: close");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(httpRequestData.length());
  client.println();
  client.println(httpRequestData);
  String fetched = http.responseBody();
  Serial2.println(fetched);
  http.stop();
  client.stop();
  Serial2.println(F("Server disconnected"));
}

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

void post_to_thingsboard() {
  delay(1000);
  if (!modemConnected) {
    Serial2.print(F("Waiting for network..."));
    if (!modem.waitForNetwork()) {
      Serial2.println(" fail");
      delay(10000);
      // return;
    }
    Serial2.println(" OK");
    Serial2.print(F("Connecting to "));
    Serial2.print(apn);
    if (!modem.gprsConnect(apn, user, pass)) {
      Serial2.println(" fail");
      delay(10000);
      // return;
    }
    modemConnected = true;
    Serial2.println(" OK");
  }

  if (!tb.connected()) {
    reconnect();
    Serial2.print("Connecting to: ");
    Serial2.print(THINGSBOARD_SERVER);
    Serial2.print(" with token ");
    Serial2.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial2.println("Failed to connect");
      // return;
    }
  }

  Serial2.println("Sending data...");
  tb.sendTelemetryFloat("energy", ENERGY);
  tb.sendTelemetryFloat("credit", creditt);
  tb.loop();
}

void reconnect() {
  while (!tb.connected()) {
    if (tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      //    tb.RPC_subscribe();
      lcd.setCursor(0, 0);
      lcd.print("  reconnected   ");
    } else {
      delay(5000);
    }
  }
}

void buss() {
  digitalWrite(buzzer, HIGH);
  delay(10);
  digitalWrite(buzzer, LOW);
}

void STS_keypad() {
  customKey = customKeypad.getKey();
  if (customKey == '*') {
    data_count = 0;
    lcd_count = 12;
    dt = 0;
    sts_value = 0;
    delay(20);
    lcd.clear();
    buss();
    lcd.setCursor(0, 0);
    lcd.print("STS MODE        ");
    sts_mode = 1;
  }

  if (sts_mode == 1 && customKey != '*' && customKey != '#' &&
      customKey != 'D') {
    if (customKey) {
      if (dt < 21) {
        delay(20);
        buss();
        dt++;
        if (dt < 5) {
          Data[data_count] = customKey;
          lcd.setCursor(lcd_count, 0);
          lcd.print(Data[data_count]);
          data_count++;
        }
        if (lcd_count > 15) {
          lcd_count = 0;
        }
        if (dt > 4) {
          Data[data_count] = customKey;
          lcd.setCursor(lcd_count, 1);
          lcd.print(Data[data_count]);
          data_count++;
        }
        lcd_count++;
        sts_value = data_count;
      }
    }
  }
  if (customKey == 'D') {
    sts_data = "";
    sts_mode = 0;
    sts_value = 0;
  }

  if (customKey == 'A' && sts_mode == 0) {
    parameters = parameters + 1;
  }

  if (customKey == '#' && sts_mode == 1 && data_count > 19) {
    sts_data = Data;
    check_tokenused();
    if (token_used == 0) {
      STStoken_decode();
    }
  }
}

void check_tokenused() {
  token_eeprom_location = mem.readLong(eeprom_location_cnt);
  if (token_eeprom_location > 4000) {
    mem.writeLong(eeprom_location_cnt, 40);
    token_eeprom_location = 40;
  }
  // token_eeprom_location = token_eeprom_location +1;
  unsigned int locationcount;
  for (locationcount = 40; locationcount < token_eeprom_location;
       locationcount++) {
    sts_eeprom_fetched = mem.readLong(locationcount);
    String ee_fetched = String(sts_eeprom_fetched);
    String sts_keyco = sts_data.substring(0, 10);
    String keyco = ee_fetched.substring(0, 10);

    long conv_keyco = keyco.toInt();
    long conv_sts_keyco = sts_keyco.toInt();
    if ((conv_keyco == conv_sts_keyco)) {
      if (c_chek == 0) {
        warntime = warn_now;
        c_chek = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("  WARNING!!!!!  ");
      lcd.setCursor(0, 1);
      lcd.print("   USED TOKEN   ");
      digitalWrite(buzzer, HIGH);
      token_used = 1;
    }
  }
}

void STStoken_decode() {
  String keycofirmation = sts_data.substring(0, 3);
  int convertekeycofirmation = keycofirmation.toInt();
  String keycofirmationmultiplier = sts_data.substring(3, 4);
  int convertekeycofirmationmultiplier = keycofirmationmultiplier.toInt();
  confirmkey = convertekeycofirmation / convertekeycofirmationmultiplier;

  String sts_meter_no_count = sts_data.substring(4, 5);
  convertedsts_data = sts_data.toInt();
  int stsnew_meter_no_count = sts_meter_no_count.toInt();
  // int stsnew_meter_no_count = sts_meter_no_count;
  String sts_day = sts_data.substring(6, 8);
  convertedsts_day = sts_day.toInt();
  String sts_third = sts_data.substring(8, 15);
  String encoder = sts_third.substring(0, 1);
  encodernew = encoder.toInt();
  String identifier = sts_third.substring(6, 7);
  String sts_encode = sts_data.substring(15, 20);
  long tariff_MT_NO = sts_encode.toInt();
  long tariff_gotten = ((tariff_MT_NO) - (meter_no * multiplier));
  long trueMT_NO = tariff_MT_NO - tariff;
  trueMT_NO = trueMT_NO / multiplier;

  if ((confirmkey == private_stskey) &&
      (convertedsts_data != eeprom_sts_data) && (trueMT_NO == meter_no) &&
      (meter_no_count == stsnew_meter_no_count)) {
    // to identify 3 digit topup
    if (identifier == "A") {
      String tp = sts_third.substring(1, 4);
      topup = tp.toInt();
      if (encoder == "1") {
        topupnew = topup + 25;
        dcode = (topupnew - topup) / 25;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "2") {
        topupnew = topup + 50;
        dcode = (topupnew - topup) / 25;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "3") {
        topupnew = topup + 39;
        dcode = (topupnew - topup) / 15;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
    }

    // to identify 4 digit topup
    if (identifier == "C") {
      String tp = sts_third.substring(1, 5);
      topup = tp.toInt();
      if (encoder == "1") {
        topupnew = topup + 15;
        dcode = (topupnew - topup) / 15;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "2") {
        topupnew = topup + 36;
        dcode = (topupnew - topup) / 18;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "3") {
        topupnew = topup + 39;
        dcode = (topupnew - topup) / 13;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
    }
    // to identify 5 digit topup
    if (identifier == "B") {
      String tp = sts_third.substring(1, 6);
      topup = tp.toInt();
      if (encoder == "1") {
        topupnew = topup + 99;
        dcode = (topupnew - topup) / 99;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "2") {
        topupnew = topup + 70;
        dcode = (topupnew - topup) / 35;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "3") {
        topupnew = topup + 60;
        dcode = (topupnew - topup) / 20;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
    }

    if (sts_accept == 1) {
      if (c_chek == 0) {
        warntime = warn_now;
        c_chek = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print(" TOKEN ACCEPTED ");
      lcd.setCursor(0, 1);
      lcd.print("   VIA KEYPAD   ");
      topup = topupnew;
      mem.writeLong(credit_eeprom_location, creditt);  // write present credit
      creditt =
          mem.readLong(credit_eeprom_location);  // fetch previous credit //
      creditt = creditt + topup;  // add new   if any to old credit //
      mem.writeLong(credit_eeprom_location, creditt);
      get_credit = 1;
    }
  }

  if (sts_accept == 0) {
    if (c_chek == 0) {
      warntime = warn_now;
      c_chek = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("  WARNING!!!!!  ");
    lcd.setCursor(0, 1);
    lcd.print("  INVALID TOKEN ");
    digitalWrite(buzzer, HIGH);
  }
}
