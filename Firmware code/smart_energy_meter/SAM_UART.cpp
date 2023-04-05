#include <SAM_UART.h>
ATM90E26_UART::ATM90E26_UART(Stream* UART) {
  ATM_UART = UART;
}
unsigned short ATM90E26_UART::AFECHIP(unsigned char RW, unsigned char address,
                                      unsigned short val) {
  unsigned short output;
  // Set read write flag
  address |= RW << 7;
  byte host_chksum = address;
  if (!RW) {
    unsigned short chksum_short = (val >> 8) + (val & 0xFF) + address;
    host_chksum = chksum_short & 0xFF;
  }

  // begin UART command
  ATM_UART->write(0xFE);
  ATM_UART->write(address);

  if (!RW) {
    byte MSBWrite = val >> 8;
    byte LSBWrite = val & 0xFF;
    ATM_UART->write(MSBWrite);
    ATM_UART->write(LSBWrite);
  }
  ATM_UART->write(host_chksum);
  delay(10);

  // Read register only
  if (RW) {
    byte MSByte = ATM_UART->read();
    byte LSByte = ATM_UART->read();
    byte atm90_chksum = ATM_UART->read();

    if (atm90_chksum == ((LSByte + MSByte) & 0xFF)) {
      output = (MSByte << 8) | LSByte;  // join MSB and LSB;
      return output;
    }
    Serial.println("Read failed");
    delay(20);  // Delay from failed transaction
    return 0xFFFF;
  }

  // Write register only
  else {
    byte atm90_chksum = ATM_UART->read();
    if (atm90_chksum != host_chksum) {
      Serial.println("Write failed");
      delay(20);  // Delay from failed transaction
    }
  }
  return 0xFFFF;
}

double ATM90E26_UART::FETCH_mains() {
  unsigned short ACvoltage = AFECHIP(1, Urms, 0xFFFF);
  return (double)ACvoltage / 100;
}

double ATM90E26_UART::FETCH_NEUTRALCurrent() {
  unsigned short neucurrent = AFECHIP(1, Irms2, 0xFFFF);
  return (double)neucurrent / 100;
}

double ATM90E26_UART::FETCH_LIVECurrent() {
  unsigned short LIVEcurrent = AFECHIP(1, Irms, 0xFFFF);
  return (double)LIVEcurrent / 1000;
}

double ATM90E26_UART::FETCH_ActivePower() {
  short int apower =
      (short int)AFECHIP(1, PmeanTwo, 0xFFFF);  // Complement, MSB is signed bit
  return (double)apower;
}

double ATM90E26_UART::FETCH_Frequency() {
  unsigned short freq = AFECHIP(1, Freq, 0xFFFF);
  return (double)freq / 100;
}

double ATM90E26_UART::FETCH_PowerFactor() {
  short int pf = (short int)AFECHIP(1, PowerFTwo, 0xFFFF);  // MSB is signed bit
  // if negative
  if (pf & 0x8000) {
    pf = (pf & 0x7FFF) * -1;
  }
  return (double)pf / 1000;
}

double ATM90E26_UART::FETCH_ImportEnergy() {
  // Register is cleared after reading
  unsigned short ienergy = AFECHIP(1, APenergy, 0xFFFF);
  return (double)ienergy * 1;  // returns kWh if PL constant set to 1000imp/kWh
}

double ATM90E26_UART::FETCH_ExportEnergy() {
  // Register is cleared after reading
  unsigned short eenergy = AFECHIP(1, ANenergy, 0xFFFF);
  return (double)eenergy *
         0.0001;  // returns kWh if PL constant set to 1000imp/kWh
}

double ATM90E26_UART::FETCH_ActiveEnergy() {
  // Register is cleared after reading
  unsigned short Aenergy = AFECHIP(1, ATenergy, 0xFFFF);
  return (double)Aenergy *
         0.0001;  // returns kWh if PL constant set to 1000imp/kWh
}

unsigned short ATM90E26_UART::FETCH_SysStatus() {
  return AFECHIP(1, SysStatus, 0xFFFF);
}

unsigned short ATM90E26_UART::FETCH_MeterStatus() {
  return AFECHIP(1, EnStatus, 0xFFFF);
}

unsigned short ATM90E26_UART::FETCH_MeterCSOne() {
  return AFECHIP(1, CSOne, 0x0000);
}

unsigned short ATM90E26_UART::FETCH_MeterCSTwo() {
  return AFECHIP(1, CSTwo, 0x0000);
}

/*
Initialise Energy IC, assume UART has already began in the main code
*/

void ATM90E26_UART::SET_register_values() {
  unsigned short systemstatus;
  AFECHIP(0, SoftReset, 0x789A);
  AFECHIP(0, FuncEn, 0x0030);
  AFECHIP(0, SagTh, 0x1F2F);

  // Set metering calibration values
  AFECHIP(0, CalStart, 0x5678);
  AFECHIP(0, PLconstH, 0x001E);  // PL Constant MSB
  AFECHIP(0, PLconstL, 0xC8B4);  // PL Constant LSB
  AFECHIP(0, Lgain, 0x1D39);     // Line calibration gain
  AFECHIP(0, Ngain, 0x1D39);     // Neutrl calibration gain
  AFECHIP(0, Lphi, 0x0000);
  AFECHIP(0, PStartTh, 0x08BD);  // Active Startup Power Threshold
  AFECHIP(0, PNolTh, 0x08BD);    // Active No-Load Power Threshold
  AFECHIP(0, QStartTh, 0x0AEC);  // Reactive Startup Power Threshold
  AFECHIP(0, QNolTh, 0x0000);    // Reactive No-Load Power Threshold
  AFECHIP(0, MMode, 0x9422);  // 9022 Metering Mode Configuration. All defaults.
                              // See pg 31 of datasheet.
  AFECHIP(0, CSOne, 0x327C);  // Write CSOne, as self calculated

  Serial.print("Checksum 1:");
  Serial.println(
      AFECHIP(1, CSOne, 0x0000),
      HEX);  // Checksum 1. Needs to be calculated based off the above values.

  // Set measurement calibration values
  AFECHIP(0, AdjStart,
          0x5678);  // Measurement calibration startup command, registers 31-3A
  AFECHIP(0, Ugain, 0x71D8);  // mt50, 0x7152//0x6720 Voltage rms gain  0x71A2
  AFECHIP(0, IgainN,
          0x0F56);  // mt50,  0x0F56//0x7530 N line current gain  0x0DC8
  AFECHIP(0, Uoffset, 0x0000);  // Voltage offset

  AFECHIP(0, IoffsetN, 0x0000);  // L line current offset
  AFECHIP(0, PoffsetN, 0x0000);  // L line active power offset
  AFECHIP(0, QoffsetN, 0x0000);  // L line reactive power offset
  AFECHIP(0, IoffsetL, 0x0000);  // L line current offset
  AFECHIP(0, PoffsetL, 0x0000);  // L line active power offset
  AFECHIP(0, QoffsetL, 0x0000);  // L line reactive power offset
  AFECHIP(0, Pmean, 0x0000);     // L Line Mean Activze Power
  AFECHIP(0, Qmean, 0x0000);     // L Line Mean Active Power
  AFECHIP(0, EnStatus, 0x4802);

  AFECHIP(1, APenergy, 0x0000);
  AFECHIP(1, ANenergy, 0x0000);
  AFECHIP(1, ATenergy, 0x0000);
  AFECHIP(
      0, CSTwo,
      0x993B);  // mt50,  0x13B5 Write CSTwo 0x6BB9, as self calculated 0x7F75

  Serial.print("Checksum 2:");
  Serial.println(
      AFECHIP(1, CSTwo, 0x0000),
      HEX);  // Checksum 2. Needs to be calculated based off the above values.

  AFECHIP(0, CalStart, 0x8765);  // Checks correctness of 21-2B registers and
                                 // starts normal metering if ok
  AFECHIP(0, AdjStart, 0x8765);  // Checks correctness of 31-3A registers and
                                 // starts normal measurement  if ok
  systemstatus = FETCH_SysStatus();
  if (systemstatus & 0xC000) {
    // checksum 1 error
    Serial.println("Checksum 1 Error!!");
  }
  if (systemstatus & 0x3000) {
    // checksum 2 error
    Serial.println("Checksum 2 Error!!");
  }
}
