void post_to_thingsboard() {
     
     delay(1000);
     if (!modemConnected) {
     Serial2.print(F("Waiting for network..."));
     if (!modem.waitForNetwork()) {
     Serial2.println(" fail");
     delay(10000);
     //return;
     }
     Serial2.println(" OK");
     Serial2.print(F("Connecting to "));
     Serial2.print(apn);
     if (!modem.gprsConnect(apn, user, pass)) {
     Serial2.println(" fail");
     delay(10000);
     //return;
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
     //return;
     }
     }

     Serial2.println("Sending data...");
     tb.sendTelemetryFloat ("energy", ENERGY);
     tb.sendTelemetryFloat ("credit", creditt);
     tb.loop();
}

void reconnect() {
  
   while (!tb.connected()){
   if ( tb.connect(THINGSBOARD_SERVER, TOKEN) ){
   //tb.RPC_Subscribe();
   lcd.setCursor(0, 0);
   lcd.print("  reconnected   ");
   }
   else{
   delay(5000);
   }
   }
}
