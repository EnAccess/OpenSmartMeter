#pragma once

// defines
#define TOKEN "5GBw6kqNCN93BN3nuuvJ"  //"YOUR_ACCESS_TOKEN"
#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define THINGSBOARD_PORT 80

// Arduino base libraries

// third party libraries
#include <ThingsBoard.h>

// OpenSmartMeter libraries
#include "lcd_init.hpp"
#include "remote.hpp"

ThingsBoard tb(client);

void reconnect() {
  while (!tb.connected()) {
    if (tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      // subscribe without callbacks
      RPC_Callback callbacks[0] = {};
      tb.RPC_Subscribe(callbacks, 0);

      lcd.setCursor(0, 0);
      lcd.print("  reconnected   ");
    } else {
      delay(5000);
    }
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
