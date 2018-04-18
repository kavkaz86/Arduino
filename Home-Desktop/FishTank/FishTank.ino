#include <OneWire.h> 
#include <LiquidCrystal.h>

int DS18S20_Pin = 8; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
OneWire ds(DS18S20_Pin); // on digital pin 2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup(void) {
 lcd.begin(16,2);
 lcd.print("Temp is:"); 
 Serial.begin(9600);
 
}

void loop(void) {
  
 lcd.setCursor(0,1);
 float temperature = getTemp();
 // Serial.println(temperature);
 lcd.print(temperature);
 lcd.print(" C"); 
 delay(1000); //just here to slow down the output so it is easier to read
 
}


float getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds.search(addr)) {
   //no more sensors on chain, reset search
   ds.reset_search();
   return -1000;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }

 ds.reset();
 ds.select(addr);
 ds.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); // Read Scratchpad

 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds.read();
 }
 
 ds.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;
 
}
