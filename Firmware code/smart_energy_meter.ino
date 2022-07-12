     // Use 0-2. Larger for more debugging messages
     #define FLASH_DEBUG  2

    // You can select another sector. Be careful not larger than (REGISTERED_NUMBER_FLASH_SECTORS - 1) and large enough not to overwrite your program
    // Default is (REGISTERED_NUMBER_FLASH_SECTORS - 1) if you don't specify here
    #define USING_FLASH_SECTOR_NUMBER   (REGISTERED_NUMBER_FLASH_SECTORS - 2)

    // To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
    #include <FlashStorage_STM32F1.h>
    const int WRITTEN_SIGNATURE = 0xBEEFDEED;
    const int START_ADDRESS     = 0;

    HardwareSerial Serial2(PA3, PA2);
    
    #include <SAM_UART.h>
    HardwareSerial ATM90E26(PB11, PB10);        
    ATM90E26_UART AFE_chip(&ATM90E26);           
                 
    
    //keypad
    #include <Keypad.h>
    #define Lengths 20
    char Data[Lengths]; 
    int lcd_count, convertedsts_data =0;
    //char firstData[Lengths];   
 
    byte data_count, data_count2, dt, sts_accept, encoder = 0;
 
    char customKey;
            
    const byte ROWS = 4;
    const byte COLS = 4;

    char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6' ,'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
    };

   byte rowPins[ROWS] = {PB9, PB8, PB1, PB0};
   byte colPins[COLS] = {PA7, PA6, PA5, PC13};

   Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
   
 
   #define TINY_GSM_MODEM_SIM800 
   #define TINY_GSM_RX_BUFFER   60  // Set RX buffer to 1Kb 
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
    HttpClient http(client, server, port);
    
    String apiKeyValue = "tPmAT5Ab3j7F9";
    String meter="MT100";
    unsigned long meter_no=100;
    unsigned long multiplier = 100; // to raise meter no to 5 digit
    unsigned int  meter_no_count = 3;
    
    unsigned int over_voltage = 245;
    unsigned int over_load = 18000;
    unsigned int low_voltage= 170;
    unsigned int low_freq = 45;
  
    unsigned int tariff = 70;
    String stat="sucess";
    String sts_data = "";
    String sts_data1 = "";
    int private_stskey = 109;
   
    
    #include <Wire.h>
    #include "RTClib.h"
    RTC_DS1307 rtc;
    char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    unsigned int hours, minutes, seconds, rtcday = 0;

   #include <LiquidCrystal.h> 
   const int rs =PB3 , en = PA15, d4 = PA8, d5 = PB15, d6 = PB14, d7 = PB13;
   LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

 
   
   #define buzzer     PB5
   #define relay      PB4
   #define b_light    PB12
   
   #define red_led    PA11
   #define pulse_led  PA4

   
   unsigned int encodernew  =  0;   
   unsigned long eepromupdate_time, eeprom_sts_data    = 0;
   float  deduction_factor, totalcurrent, avergecurrent, topup, creditt =0.0;
   float lcd_creditt, totalbill, energy_billing, amphour, dcode, ENERGY, PF, freq =0.0;
   word prev_energypulse, new_energypulse=0;

   long  topupnew =0;


   unsigned long  set_mains_wattage, lcd_refresh,lcd_reset,mains_inputV,cnt  = 000;
   unsigned long  set_inv_wattage, pv_v, u, mains_v, j, h, m,  sts_mode= 0;
   double cu, dc, t_cu, btt, currinv_adc, currmains_adc, p_mains, curr_mains_Value, curr_inv_Value, init_curr,billing,curr,true_power = 0.0;
   double pvvalue, mainsvalue, curinv_v, curnep_v, relcur, but, bat_curr, bat_curr_value, p_inv, inv_power, mains_power,mains_input_value   = 0.000;
   byte internetpost_time, internetget_time, internetenergy_time,  credit_time,sucess=0;
   unsigned long power, sts_value, convertedsts_day =0;
   unsigned long milisec, p_time, instant_ENERGY = 0;
   unsigned long impulse_rate = 1000;
   unsigned int pulse, pulse_set=0;
   unsigned int pulse_delay=200;
   unsigned long pulsetime,  current_time, previous, previousenergytime, energytime, currentenergytime  = 0;
   byte get_credit, fault=0;
   int confirmkey =0;


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
   pinMode (buzzer,     OUTPUT);
   pinMode (relay,      OUTPUT); 
   pinMode (b_light,    OUTPUT);
   pinMode (red_led,    OUTPUT);
   pinMode (pulse_led,  OUTPUT);
   
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

    
   ATM90E26.begin(9600);
   AFE_chip.SET_register_values();
   delay(1000); 

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
    
   lcd.setCursor(0, 1);
   lcd.print(" #####          ");
   
   //Serial2.println("Initializing modem...");
   modem.restart();
   String modemInfo = modem.getModemInfo();
   Serial2.print("Modem: ");
   Serial2.println(modemInfo); 
 
   lcd.setCursor(0, 1);
   lcd.print(" ###############");
   delay(3000); 
   lcd.clear(); 


   if (! rtc.begin()) {
   lcd.print("Couldn't find RTC");
   delay(2000);
   while (1);  
   }
   if (! rtc.isrunning()) {
   lcd.print("RTC is NOT running!");
   delay(2000);
   }
     
   lcd.clear();
   EEPROM.init(); 
   EEPROM.setCommitASAP(false);
   EEPROM.get(0, creditt);      // fetch previous credit //
   if(isDigit(creditt)){
   EEPROM.put(0, creditt);
   }
   else {
   EEPROM.put(0, 0);
   }
}

 


void loop() {
  
   mesure();
   credit_formular();
   credit_reminder();  
   if(sts_mode==0){
   lcd_update();
   gsm_func();
   }
   STS_keypad();
   get_time();
   
   
    
    
}



void lcd_update(){
  
   cnt = cnt+1;
   if(cnt>0 && cnt<2){
   lcd.setCursor(0, 0);
   lcd.print("V=   ");
   lcd.setCursor(2, 0);
   lcd.print(mains_input_value);
   //lcd.setCursor(6, 0);
   //lcd.print("V");
   
   lcd.setCursor(7, 0);
   lcd.print(" ");
  
   lcd.setCursor(8, 0);
   lcd.print("C=     ");
   lcd.setCursor(10, 0);
   lcd.print(curr);
   lcd.setCursor(15, 0);
   lcd.print("A");
   } 
   lcd.setCursor(0, 1);
   lcd.print(" CREDIT=#       ");
   lcd.setCursor(9, 1);
   lcd.print(creditt);
  
   
   if(cnt>1 && cnt<3){
   lcd.setCursor(0, 0);
   lcd.print("POWER=          ");
   lcd.setCursor(6, 0);
   lcd.print(true_power);
   lcd.setCursor(14, 0);
   lcd.print("WT"); 

   lcd.setCursor(0, 1);
   lcd.print("ENERGY=      KWH");
   lcd.setCursor(7, 1);
   lcd.print(ENERGY);
   }
   if(cnt>2){
   cnt=0;
   }   
}



void mesure() {

   Serial2.println("Power Factor:");
   Serial2.print(AFE_chip.FETCH_PowerFactor());
   PF = AFE_chip.FETCH_PowerFactor();
   delay(2);

   Serial2.println("freq:");
   Serial2.print(AFE_chip.FETCH_Frequency());
   freq = AFE_chip.FETCH_Frequency();
   delay(2);
   
   Serial2.println("Voltage:"); 
   mainsvalue = AFE_chip.FETCH_mains();   
   mains_input_value = mainsvalue*0.47630619684082624544349939246659;//*0.4871672550329167064211430207041;
   Serial2.print(mains_input_value);
   
   
   Serial2.println("Current:"); 
   curr = AFE_chip.FETCH_NEUTRALCurrent();
   curr= curr*0.22296544102564102564102564102564;
   if(curr<0.4){
   curr=0;
   }
   Serial2.print(curr);   
   delay(2);
   
   Serial2.println("Active power:");
   true_power = AFE_chip.FETCH_ActivePower();
   true_power = curr*mains_input_value*PF;
   Serial2.print(true_power);
   delay(2);
   
   digitalWrite(pulse_led, LOW);   // turn the LED on (HIGH is the voltage level)
   prev_energypulse = ENERGY*1000;
   previousenergytime = millis()/1000;
   energytime = previousenergytime - currentenergytime;
   
    ENERGY = ENERGY +  ( ( true_power * energytime) / (  3600000 ) ); // energy per KWH(energy gotten per sec)
   Serial2.println("ENERGY:");
    Serial2.print(ENERGY);
   currentenergytime = millis()/1000;
   new_energypulse = ENERGY*1000;
   if(new_energypulse>prev_energypulse ){
   digitalWrite(pulse_led, HIGH); 
   }

   if ( (freq<low_freq) || (mains_input_value > over_voltage) || (mains_input_value < low_voltage) || (true_power > over_load) ){
   fault = 1;    
   }  
    if ( (freq>low_freq) || (mains_input_value < over_voltage) || (mains_input_value > low_voltage) || (true_power < over_load) ){
   fault = 0;    
   } 
}
  


void credit_formular(){ 
 
   if(get_credit == 1){
   EEPROM.get(0, creditt);      // fetch previous credit //
   delay(2);
   get_credit = 0;
   }
   if(true_power >0){
   billing = (ENERGY * tariff)/3600 ;  // cost per sec
   if( creditt > billing ){
   creditt = ( creditt - billing );
   }
   delay(2); 
   }
}


void credit_reminder(){  
    
   if(creditt>10 && creditt<200){
   digitalWrite(red_led, HIGH);   
   delay(120);                       
   digitalWrite(red_led, LOW);    
   delay(120);   
   }
   if(creditt<10 || fault == 1 ){
   digitalWrite(red_led, HIGH);                               
   }
   if(creditt>0){
   digitalWrite(relay, HIGH);  //meter output on //                                 
   }
   if(creditt<1 && fault == 0){
   digitalWrite(relay, LOW);  //meter output off //                                 
   }
   if(creditt>200 && fault == 0){ 
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
  
         
   http.stop();
   client.stop();
   Serial2.println(F("Server disconnected"));
   modem.gprsDisconnect();
   Serial2.println(F("GPRS disconnected"));  

  


   if(topup>0){    // if topup available //
   post_sucess();  // update sucess //
   lcd.setCursor(0, 0);
   lcd.print(" TOKEN RECEIVED ");
   lcd.setCursor(0, 1);
   lcd.print("     VIA WEB    ");
   // add topup to eeprom // 
   //EEPROM.get(0, creditt);      // fetch previous credit //
   creditt = creditt + topup;   // add new   if any to old credit //
   EEPROM.put(0, creditt);   // update newly added topup value //
   get_credit = 1;
   if (!EEPROM.getCommitASAP())
   {
   EEPROM.commit();
   }
   delay(1500);  
   }
   topup=0;   // reset topup value back to 0//
   }
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
   String httpRequestData = "api_key=" + apiKeyValue + "&stat=" + String(stat)+ "&meter=" + String(meter) + "";
   
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


void get_time(){

   DateTime now = rtc.now(); 
   hours =    (now.hour());
   minutes = (now.minute());
   seconds = (now.second());
   rtcday = (now.day());
   
   if(hours==11){
   //post_to_thingsboard();
   
   //update credit at the end of the day
   EEPROM.put(0, creditt);   
   if (!EEPROM.getCommitASAP())
   {
   EEPROM.commit();
   }
   }   
   
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
   tb.sendTelemetryFloat("power", ENERGY);
   tb.sendTelemetryFloat("credit", creditt);
   tb.loop();
   ENERGY=0;
}






void buss (){
   
   digitalWrite(buzzer, HIGH);
   delay(50);
   digitalWrite(buzzer, LOW);
   delay(50);
}



void STS_keypad(){
   
   customKey = customKeypad.getKey();
   if(customKey == '*'){
   data_count = 0;
   lcd_count = 12;
   //data_count2 = 0;
   dt=0;
   sts_value=0;
   delay(100);
   lcd.clear(); 
   buss ();
   lcd.setCursor(0, 0);
   lcd.print("STS MODE        ");
   sts_mode=1;
   }
   
   if(  sts_mode==1 && customKey != '*' && customKey != '#' && customKey != 'D'){
   if(customKey ){
   if(dt<21){
   delay(100);
   buss ();   
   dt++;
  
   
   if(dt<5){
   Data[data_count] = customKey; 
   lcd.setCursor(lcd_count, 0);
   lcd.print(Data[data_count]);
   data_count++;
   }
   if(lcd_count > 15){
   lcd_count =0;
   }
   if(dt>4){
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
   if(customKey == 'D' ){
   sts_data="";
   sts_mode=0;  
   sts_value=0; 
   }
    
   if(customKey == '#' &&  sts_mode==1 && data_count>19 ){
   sts_data = Data;
   STStoken_decode();
   }
    
}

   
void STStoken_decode(){
  
   String keycofirmation= sts_data.substring(0,3);
   int convertekeycofirmation = keycofirmation.toInt(); 
   String keycofirmationmultiplier= sts_data.substring(3,4);
   int convertekeycofirmationmultiplier = keycofirmationmultiplier.toInt();
   confirmkey = convertekeycofirmation/convertekeycofirmationmultiplier;

   String sts_meter_no_count= sts_data.substring(4,5);
   convertedsts_data = sts_data.toInt(); 
   int stsnew_meter_no_count = sts_meter_no_count.toInt(); 
   //int stsnew_meter_no_count = sts_meter_no_count;
   String sts_day= sts_data.substring(6,8);
   convertedsts_day=sts_day.toInt(); 
   String sts_third= sts_data.substring(8,15);
   String encoder= sts_third.substring(0,1);
   encodernew = encoder.toInt(); 
   String identifier= sts_third.substring(6,7);
   String sts_encode= sts_data.substring(15,20);
   long tariff_MT_NO = sts_encode.toInt(); 
   long tariff_gotten=( (tariff_MT_NO) - (meter_no*multiplier) );
   long trueMT_NO = tariff_MT_NO - tariff;
   trueMT_NO =  trueMT_NO/multiplier;

   
   if ( (rtcday==convertedsts_day) && (confirmkey == private_stskey) && ( convertedsts_data != eeprom_sts_data ) && (trueMT_NO == meter_no) && (meter_no_count == stsnew_meter_no_count) ){
    
   //to identify 3 digit topup
   if (  identifier== "A"){
   String tp =   sts_third.substring(1,4);
   topup=tp.toInt(); 
   if(encoder  ==  "1"){
   topupnew  = topup + 25;
   dcode = (topupnew -  topup)/25;
   if( dcode == encodernew){
   sts_accept=1;
   }
   else if(dcode != encodernew){
   sts_accept=0;  
   }
   } 

   if(encoder  ==  "2"){
   topupnew  = topup + 50;
   dcode = (topupnew -  topup)/25;
   if( dcode == encodernew){
   sts_accept=1;
   }
   else if(dcode != encodernew){
   sts_accept=0;  
   }
   } 

   if(encoder  ==  "3"){
   topupnew  = topup + 39;
   dcode = (topupnew -  topup)/15;
   if( dcode == encodernew){
   sts_accept=1;
   }
   else if(dcode != encodernew){
   sts_accept=0;  
   } 
   } 
   }
   
   //to identify 4 digit topup
   if ( identifier  == "C"){
   String tp =   sts_third.substring(1,5);
   topup=tp.toInt(); 
   if(encoder  ==  "1"){
   topupnew  = topup + 15;
   dcode = (topupnew -  topup)/15;
   if( dcode == encodernew){
   sts_accept=1;
   }
   else if(dcode != encodernew){
   sts_accept=0;  
   }
   } 

   if(encoder  ==  "2"){
   topupnew  = topup + 36;
   dcode = (topupnew -  topup)/18;
   if( dcode == encodernew){
   sts_accept=1;
   } 
   else if(dcode != encodernew){
   sts_accept=0;  
   }  
   } 

   if(encoder  ==  "3"){
   topupnew  = topup + 39;
   dcode = (topupnew -  topup)/13;
   if( dcode == encodernew){
   sts_accept=1;
   }
   else if(dcode != encodernew){
   sts_accept=0;  
   } 
   } 
   }
   
   //to identify 5 digit topup
   if ( identifier == "B"){
   String tp =   sts_third.substring(1,6);
   topup=tp.toInt(); 
   if(encoder  ==  "1"){
   topupnew  = topup + 99;
   dcode = (topupnew -  topup)/99;
   if( dcode == encodernew){
   sts_accept=1;
   }
   else if(dcode != encodernew){
   sts_accept=0;  
   }
   } 

   if(encoder  ==  "2"){
   topupnew  = topup + 70;
   dcode = (topupnew -  topup)/35;
   if( dcode == encodernew){
   sts_accept=1;
   } 
   else if(dcode != encodernew){
   sts_accept=0;  
   }
   } 

   if(encoder  ==  "3"){
   topupnew  = topup + 60;
   dcode = (topupnew -  topup)/20;
   if( dcode == encodernew){
   sts_accept=1;
   }
   else if(dcode != encodernew){
   sts_accept=0;  
   } 
   } 
   }
   

   
   if( sts_accept==1 ){
   lcd.setCursor(0, 0);
   lcd.print(" TOKEN ACCEPTED ");
   lcd.setCursor(0, 1);
   lcd.print("   VIA KEYPAD   ");
   delay(1500);  
   //creditt = topupnew;
   topup = topupnew;
   //EEPROM.get(0, creditt);      // fetch previous credit //
   creditt = creditt + topup;   // add new   if any to old credit //
   EEPROM.put(0, creditt);   // update newly added topup value //
   get_credit = 1;
   if (!EEPROM.getCommitASAP())
   {
    EEPROM.commit();
   }
   }
   }
   
   if( sts_accept==0 ){
   lcd.setCursor(0, 0);
   lcd.print("  WARNING!!!!!  ");
   lcd.setCursor(0, 1);
   lcd.print("  INVALID TOKEN ");
//   lcd.setCursor(0, 1);
//   lcd.print("                 ");
//   lcd.setCursor(0, 1);
//   lcd.print(stsnew_meter_no_count);
   digitalWrite(buzzer, HIGH);
   delay(1500);  
   digitalWrite(buzzer, LOW);
   }

   if( convertedsts_data == eeprom_sts_data ){
   lcd.setCursor(0, 0);
   lcd.print("  WARNING!!!!!  ");
   lcd.setCursor(0, 1);
   lcd.print("   USED TOKEN   ");
   digitalWrite(buzzer, HIGH);
   delay(1500);  
   digitalWrite(buzzer, LOW);
   }
   
   // save topup to eeprom
   sts_data="";
   sts_mode=0; 
   data_count=0; 
   sts_value=0;  
   sts_accept=0;
   topupnew=0;
   topup=0;
} 
 
