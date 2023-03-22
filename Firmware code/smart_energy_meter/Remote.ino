void gsm_func() {

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


     String   s_received_credit, s_tariff,readString ; 
     byte ind1;   
     byte ind2;
     String body = http.responseBody();
     readString=body; 
     ind1 =  readString.indexOf("/");  //finds location of first
     s_received_credit = readString.substring(0, ind1);  // cpture received credit
     ind2 = readString.indexOf("/", ind1+1 );   //finds location of second,
     s_tariff = readString.substring(ind1+1, ind2+1);   //captures second data String
     
     Serial2.println(F("topup:"));
     topup = s_received_credit.toFloat();
     Serial2.println(topup);
     tariff = s_tariff.toInt();
     http.stop();
     client.stop();
     Serial2.println(F("Server disconnected"));
      
  
  
     if(topup>0){    // if topup available //
     post_sucess();  // update sucess //
     tp_fetch=1;
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
     lcd.print( topup );
     // add topup to eeprom //
     mem.writeLong(credit_eeprom_location, creditt); 
     creditt = mem.readLong(credit_eeprom_location);      // fetch previous credit //
     creditt = creditt + topup;   // add new   if any to old credit //
     mem.writeLong(credit_eeprom_location, creditt);  // update newly added topup value // 
     delay(1500);
     tp_fetch=0;
     }
     topup=0;   // reset topup value back to 0//
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
     Serial2.println(ENERGY );
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
     String fetched = http.responseBody();
     Serial2.println( fetched );
     http.stop();
     client.stop();
     Serial2.println(F("Server disconnected"));
}
