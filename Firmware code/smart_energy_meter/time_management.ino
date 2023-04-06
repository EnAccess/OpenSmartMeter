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
