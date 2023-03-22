   HardwareSerial Serial2(PA3, PA2);
   #include <SAM_UART.h>
   HardwareSerial ATM90E26(PB11, PB10);
   ATM90E26_UART AFE_chip(&ATM90E26);

   float   powerFactor, freq, mains_input_value, curr, true_power, ENERGY, creditt = 0;

void setup() {
  Serial2.begin(115200);
  ATM90E26.begin(9600);
  AFE_chip.SET_register_values();
  delay(1000);
}

void loop() {

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
     delay(1500); 
}
