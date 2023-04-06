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

void buss() {
  digitalWrite(buzzer, HIGH);
  delay(10);
  digitalWrite(buzzer, LOW);
}
