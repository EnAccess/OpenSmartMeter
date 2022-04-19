    //keypad
    #include <Keypad.h>
    #define Lengths 20
    char Data[Lengths]; 
 
    byte data_count = 0, master_count = 0;
 
    char customKey;

    const byte ROWS = 4;
    const byte COLS = 4;

    char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6' ,'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
    };

   byte rowPins[ROWS] = {PB10, PB2, PB1, PB0};
   byte colPins[COLS] = {PA7, PA6, PA5, PA4};

   Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
   
   HardwareSerial Serial2(2);
   #define TINY_GSM_MODEM_SIM800 
   #define TINY_GSM_RX_BUFFER   255  // Set RX buffer to 1Kb 
   #include <TinyGsmClient.h>
   #include <ArduinoHttpClient.h>
   #include "ThingsBoard.h"
   
   const char apn[]  = "web.gprs.mtnnigeria.net";
   const char user[] = "";
   const char pass[] = "";
   #define TOKEN             "w6Hmb6r2wPv1fLa8ZN77"  //"YOUR_ACCESS_TOKEN"
   #define THINGSBOARD_SERVER  "demo.thingsboard.io"
   TinyGsm modem(Serial1);
   TinyGsmClient client(modem);
   ThingsBoard tb(client);
   bool modemConnected = false;


    // SIM card PIN (leave empty, if not defined)
    const char simPIN[]   = ""; 

     
    const char server[] = "paygotesting.000webhostapp.com"; // domain name: example.com, maker.ifttt.com, etc
    const char resource[] = "/subscribe.php";         // resource path, for example: /post-data.php
    const int  port = 80;                             // server port number
    HttpClient    http(client, server, port);
    
    String apiKeyValue = "tPmAT5Ab3j7F9";
    String value1="message from STM32";
    String meter="MT100";
    String stat="sucess";
   
   
   
   #include <EEPROM.h>
    #include <Wire.h>
  #include "RTClib.h"
  RTC_DS1307 rtc;
  char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  int hours, minutes, seconds = 0;
   
   int hr, minut, secs, credit_timer = 0;
   bool x1 ,x2 ,x3 ,x4 ,x5 ,x6 ,x7 ,x8 ,x9 ,x10 ,x11 ,x12,gotten,done =0;

   #include <LiquidCrystal.h> 
   const int rs =PB3 , en = PA15, d4 = PA8, d5 = PB15, d6 = PB14, d7 = PB13;
   LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

   #define inv_curr  PA0
   #define ac_volt   PA1
   
   #define buzzer     PC13
   #define relay      PC14
   #define b_light    PB12
   
   #define red_led    PB4
   #define green_led  PB5

   int no_load_curr = 395;
   
   long eepromupdate_time    = 0;
   //long creditt=0;
   float topup, creditt, deduction_factor, totalcurrent, avergecurrent=0;
   float lcd_creditt, totalbill, energy_billing, energy, amphour =0;
   long power_t   = 0000000;
   long tariff, received_credit=0;
   long recharge  = 0;
   
 
   long powerConsumed = 0;
   long remainingCredit = 0;
    


   int calib      = 627;
   char incomingByte;
   String readBuffer = "";
   
   String   s_received_credit, abbey_value, splitted, mt, s_tariff,readString,id; 

   byte ind2;
   byte ind3;
   byte ind4;
   
   unsigned int long period = 0;
   unsigned int long period1 = 2000;
   unsigned int long period2 = 2000;
   unsigned long time_now1, prev_time1=0;
   unsigned long  set_mains_wattage, lcd_refresh,lcd_reset,mains_inputV,cnt  = 000;
   unsigned long  set_inv_wattage, pv_v, u, mains_v, j, h, m, mains_input_value, sts_mode= 0;
   double cu, dc, t_cu, btt, currinv_adc, currmains_adc, p_mains, curr_mains_Value, curr_inv_Value, init_curr,billing,curr,true_power = 0.0;
   double pvvalue, mainsvalue, curinv_v, curnep_v, relcur, but, bat_curr, bat_curr_value, p_inv, inv_power, mains_power,calibration_factor   = 0.000;
   byte internetpost_time, internetget_time, internetenergy_time,  credit_time,sucess=0;
   long power, sts_value =0;



   byte fe1[8] ={
   0b00011,
   0b00011,
   0b00011,
   0b00011,
   0b00011,
   0b11111,
   0b11111,
   0b11111
   };
    
   byte fe3[8] ={
   0b11111,
   0b11111,
   0b11111,
   0b00011,
   0b00011,
   0b00011,
   0b00011,
   0b00011
   };

   byte fe2[8] ={
   0b11111,
   0b11111,
   0b00000,
   0b00000,
   0b00000,
   0b01100,
   0b01100,
   0b01111
   };

   byte fe4[8] ={
   0b01111,
   0b01100,
   0b01100,
   0b00000,
   0b00000,
   0b00000,
   0b00000,
   0b00000
   };




void setup() {
  
   lcd.begin(16, 2);
   Serial2.begin(115200);
   Serial1.begin(115200);
    
   Serial2.print(F(" working"));
   delay(3000);  
   Serial1.write("AT+IPR=9600\r\n");
   Serial1.end();
   Serial1.begin(9600);
   
   Serial2.println(F("Initializing modem..."));
   modem.restart();
   String modemInfo = modem.getModemInfo();
   Serial2.print(F("Modem: "));
   Serial2.println(modemInfo); 

   
   
  
   pinMode (buzzer,     OUTPUT);
   pinMode (relay,      OUTPUT); 
   pinMode (b_light,    OUTPUT);
   pinMode (red_led,    OUTPUT);
   pinMode (green_led,  OUTPUT);
   
   digitalWrite(buzzer,  HIGH);  
   digitalWrite(b_light, HIGH);  
 



    
   lcd.createChar(0,fe1);
   lcd.createChar(1,fe2);
   lcd.createChar(2,fe3);
   lcd.createChar(3,fe4);
   
   
    
   
   lcd.setCursor(0, 0);
   lcd.print("  DEVELOPED BY  ");

   lcd.setCursor(0, 1);
   lcd.print(" FIRST ELECTRIC ");
   delay(2000); 
   digitalWrite(buzzer, LOW);
   lcd.clear();
   lcd.setCursor(7, 0);
   lcd.write(byte(0));
   lcd.setCursor(8, 0);
   lcd.write(byte(1));
   lcd.setCursor(7, 1);
   lcd.write(byte(2));
   lcd.setCursor(8, 1);
   lcd.write(byte(3));
   delay(3000);
   
   lcd.setCursor(0, 0);
   lcd.print(" SYSTEM BOOTING ");

   lcd.setCursor(0, 1);
   lcd.print(" ##             ");
   delay(3000);
    
   lcd.setCursor(0, 1);
   lcd.print(" #####          ");
   delay(2000); 
   
   lcd.setCursor(0, 1);
   lcd.print(" ##########     ");
   delay(2000); 
   
   lcd.setCursor(0, 1);
   lcd.print(" ###############");
   delay(3000); 
   lcd.clear(); 

   if (! rtc.begin()) {
   Serial2.print("Couldn't find RTC");
   lcd.print("Couldn't find RTC");
   delay(2000);
   while (1);  
   }

   if (! rtc.isrunning()) {
   Serial2.print("RTC is NOT running!");
   lcd.print("RTC is NOT running!");
   //delay(2000);
   }
 
}

 


void loop() {

  
   if(sts_mode==0){
   lcd_update();
   }
   mesure(); 
   gsm_func();
   credit_formular();
   credit_reminder();
   get_time();


   long milisec = millis();
   long p_time = milisec/1000;
   long eepromupdate_time = milisec/1000;

   
   totalcurrent = totalcurrent+curr;
   avergecurrent = totalcurrent/p_time;
   amphour = (avergecurrent*p_time)/3600;
   energy = ( (true_power*p_time)/(3600) );
   energy_billing = ( (true_power*eepromupdate_time)/(3600) );
   if(p_time>86400){
   p_time=0;
   energy=0;
   }

   STS_keypad();

   
}

void STS_keypad(){
    
   customKey = customKeypad.getKey();
   
   if(customKey == '*'){
   data_count = 0;
   delay(100);
   lcd.clear(); 
   buss ();
   lcd.setCursor(0, 0);
   lcd.print("    STS MODE    ");
   sts_mode=1;
   }
   
   if(sts_mode==1 && customKey != '*'){
   if(customKey ){
   delay(50);
   buss ();
   Data[data_count] = customKey; 
   lcd.setCursor(data_count, 1);
   lcd.print(Data[data_count]);
   data_count++;
   }
   }
    
   
 
}
   
  


void buss (){
   
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
 }


void lcd_update(){
  
   cnt = cnt+1;
   delayMicroseconds(2);
   if(cnt>0 && cnt<8){
   lcd.setCursor(0, 0);
   lcd.print("V=   ");
   lcd.setCursor(2, 0);
   lcd.print(mains_input_value);
   lcd.setCursor(5, 0);
   lcd.print("V");
   
   lcd.setCursor(6, 0);
   lcd.print("  ");
  
   lcd.setCursor(8, 0);
   lcd.print("C=     ");
   lcd.setCursor(10, 0);
   lcd.print(curr);
   lcd.setCursor(15, 0);
   lcd.print("A");
   prev_time1 = time_now1;  
   } 
   lcd.setCursor(0, 1);
   lcd.print(" CREDIT=#       ");
   lcd.setCursor(10, 1);
   lcd.print(creditt);
    
    
   
   if(cnt>8 && cnt<16){
   lcd.setCursor(0, 0);
   lcd.print("POWER=          ");
   lcd.setCursor(6, 0);
   lcd.print(true_power);
   lcd.setCursor(11, 0);
   lcd.print("WATT");  
   prev_time1 = time_now1;
   }
   if(cnt>16){
   cnt=0;
   }   
}


void mesure() {
   
   curinv_v=0;
   for (j = 0x00; j <5000 ; j++)    {
   currinv_adc= analogRead( inv_curr );
   delayMicroseconds(1);    // delay for stability 
   curinv_v    = curinv_v+ currinv_adc;    
   }
   curinv_v = (curinv_v/5000);
   curr_inv_Value = curinv_v;
  
   //calib = no_load_curr*1024;
   if(curr_inv_Value >  calib){
   relcur = curr_inv_Value - calib;
   }
   if(curr_inv_Value <= calib ) {
   relcur = 0;
   }
  
   curr=  relcur*0.02545454545454545454545454545454;
   true_power = curr*mains_input_value;
   //energy = true_power;



   // to measure mains supply voltage //
   mains_v=0;
   for (h = 0x00; h<5000 ; h++)    {
   mainsvalue = analogRead(ac_volt);               
   delayMicroseconds(2);    // delay for stability 
   mains_v    = mains_v+  mainsvalue;     
   }
   mains_v = (mains_v/5000);
   mains_input_value = mains_v*0.42556012802926383173296753543667;
    
   if ( mains_input_value<30){
   mains_input_value=0;
   }

   if ( mains_input_value>235){
   digitalWrite(buzzer,  LOW); 
   }

   if ( mains_input_value<170){
   digitalWrite(buzzer,  LOW); 
   }
}
  

void credit_formular(){ 

   deduction_factor = tariff/1000;
   billing = (deduction_factor * energy_billing)/1000;
   billing = billing + billing;
 
   if(eepromupdate_time>1800){ //update eeprom every 30 mins //
   billing = billing/1800;
   EEPROM.get(0, creditt);      // fetch previous credit //
   if( creditt > billing ){
   creditt = ( creditt - billing );
   }
   EEPROM.update(0, creditt);   // update  subtracted value // 
   eepromupdate_time=0;
   energy_billing=0;
   billing=0;
   }
}




void credit_reminder(){  
    
   if(creditt>10 && creditt<200){
   digitalWrite(red_led, HIGH);  
   digitalWrite(green_led, LOW); 
   delay(500);                       
   digitalWrite(red_led, LOW);    
   delay(500);   
   }
  
   if(creditt<10){
   digitalWrite(red_led, HIGH);
   digitalWrite(relay, HIGH);   // meter output off //
   delay(100);                               
   }
   if(creditt>10){
   digitalWrite(relay, LOW);  //meter output on //                                 
   }
  
   if(creditt>200){
   digitalWrite(green_led, HIGH);   
   digitalWrite(red_led, LOW);                           
   }
}



void gsm_func(){ 
    
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
    }
    else {
    Serial2.println(" OK");
    }

    Serial2.println("Performing HTTP POST request...");
    String httpRequestData = "api_key=" + apiKeyValue + "&meter=" + String(meter) + "";
   
    client.print(String("POST ") + resource + " HTTP/1.1\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(httpRequestData.length());
    client.println();
    client.println(httpRequestData);
    
    String body = http.responseBody();
    Serial2.println(F("topup:"));
    topup = body.toFloat();
    Serial2.println(topup);
    //topup=topup+500;
    Serial2.println(topup);
         
    http.stop();
    client.stop();
    Serial2.println(F("Server disconnected"));
    modem.gprsDisconnect();
    Serial2.println(F("GPRS disconnected"));  

    // add topup to eeprom // 
    EEPROM.get(0, creditt);      // fetch previous credit //
    creditt = creditt + topup;   // add new   if any to old credit //
    EEPROM.update(0, creditt);   // update newly added topup value //

    if(topup>0){    // if topup available //
    post_sucess();  // update sucess //
    }
    topup=0;   // reset topup value back to 0//
}


void post_sucess(){


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
    }
    else {
    Serial2.println(" OK");
    }

     
    Serial2.println("Performing HTTP POST request...");
    String httpRequestData = "api_key=" + apiKeyValue + "&sucess=" + String(sucess)+ "&meter=" + String(meter) + "";
   
    client.print(String("POST ") + resource + " HTTP/1.1\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(httpRequestData.length());
    client.println();
    client.println(httpRequestData);
    
    String body = http.responseBody();
    Serial2.println( body );
    
    http.stop();
    client.stop();
    Serial2.println(F("Server disconnected"));
    modem.gprsDisconnect();
    Serial2.println(F("GPRS disconnected"));  
}


void post_to_thingsboard(){
  
   delay(1000);
   if (!modemConnected) {
   Serial2.print(F("Waiting for network..."));
   if (!modem.waitForNetwork()) {
   Serial2.println(" fail");
   delay(10000);
   return;
   }
   Serial2.println(" OK");
   Serial2.print(F("Connecting to "));
   Serial2.print(apn);
   if (!modem.gprsConnect(apn, user, pass)) {
   Serial2.println(" fail");
   delay(10000);
   return;
   }
   modemConnected = true;
   Serial2.println(" OK");
   }
   if (!tb.connected()) { 
   Serial2.print("Connecting to: ");
   Serial2.print(THINGSBOARD_SERVER);
   Serial2.print(" with token ");
   Serial2.println(TOKEN);
   if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
   Serial2.println("Failed to connect");
   return;
   }
   }
   Serial2.println("Sending data...");
   tb.sendTelemetryFloat("power", true_power);
   tb.sendTelemetryFloat("credit", creditt);
   tb.loop();

  }


void get_time(){

   DateTime now = rtc.now(); 
   
   hours = (now.hour());
   minutes = (now.minute());
   seconds = (now.second());
//   if(hours==12){
//   post_to_thingsboard();
//   }  

   lcd.setCursor(0, 0);
   lcd.print("H" );
   lcd.setCursor(0, 1);
   lcd.print( hours );

   lcd.setCursor(5, 0);
   lcd.print("M" );
   lcd.setCursor(5, 1);
   lcd.print( minutes );

   lcd.setCursor(9, 0);
   lcd.print("S" );
   lcd.setCursor(9, 1);
   lcd.print( seconds );
      
}

   void STStoken_decode(){

    
  
  }
  
