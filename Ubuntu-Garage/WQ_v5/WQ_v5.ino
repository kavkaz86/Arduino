/*
**********************************************************************************************
 *                INCLUDE FILES
 **********************************************************************************************
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <OneWire.h>

/*  INPUT PINS            */
const int TURB = A0; //TURBIDITY SENSOR ANALOG PIN = A0
const int PAR =A1;  //PAR SENSOR ANALOG PIN = A1
const int UV = A2;  //UV SENSOR ANALOG PIN = A2 
const int TEMP_PIN = 9; //TEMPERATURE SENSOR DIGITAL PIN = 9
byte i;
byte present = 0;
byte data[12];
byte addr[8];

OneWire  ds(TEMP_PIN);

void DS18B20_INIT(void){
  
  if ( !ds.search(addr)) {
      delay(1000);
      ds.reset_search();
      return;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      delay(1000);
      return;
  }
}

void setup(){
  
  Serial.begin(9600); 
  pinMode(UV, INPUT); 
  pinMode(PAR, INPUT);
  pinMode(TURB, INPUT);
  DS18B20_INIT();
  analogReference(INTERNAL);
  
}

void loop(){
  
  
  

}


float capture_uv(int pin){
  
  int pin_adc = analogRead(pin); 
  
  float voltage = (1100 * pin_adc)/1023;
  
  return(voltage); 
}

float capture_par(int pin){
  
  int pin_adc = analogRead(pin); 
  
  float voltage = (1100 * pin_adc)/1023;
  
  return(voltage); 
}

float capture_turb(int pin){
  
  int pin_adc = analogRead(pin); 
  
  float voltage = (1100 * pin_adc)/1023;
  
  return(voltage); 
}

int capture_temp(void) {
  int HighByte, LowByte, SignBit, Whole, Fract, TReading, Tc_100;
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  
  if (SignBit) {
    TReading = -TReading;
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  Whole = Tc_100 / 100;          // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
  if (Fract > 49) {
    if (SignBit) {
      --Whole;
    } else {
      ++Whole;
    }
  }
  
  return(Whole);
}

