#include <OneWire.h>
#include "Streaming.h"
#include "XBee.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>   


XBee xbee = XBee(); 
/*
ZBTxRequest ztx = ZBTxRequest(co, (uint8_t *)str, sizeof(str));
xbee.send(ztx);
*/
XBeeAddress64  co = XBeeAddress64(0x00000000, 0x00000000);

/*  SENSOR UNITS           */
#define  UV_DEL       "C:"
#define  NODE_ID      "WQ2";
#define  SEP          '#'
#define  PAR_DEL      "D:"
#define  TUR_DEL      "H:"
#define  TEMP_DEL     "A:"
#define  END_OF_TX    '\r'
#define  emsg         '-'
#define  sleep_pin    3 
#define  TX_DELAY     100
#define  SENSE_DELAY  500
#define  rf_buffer  100
/*  INPUT PINS            */
const int UV_PIN = A1;  //UV SENSOR ANALOG PIN = A1  
const int PAR_PIN =A2;  //PAR SENSOR ANALOG PIN = A2 
const int TUR_PIN = A3; //TURBIDITY SENSOR ANALOG PIN = A3 
const int TEMP_PIN = 2; //TEMPERATURE SENSOR DIGITAL PIN = 9 
OneWire  ds(2);


String inputstring = "";         
String tx = "";
String s1=""; // temp
String s2=""; //turb
String s3=""; //uv
String s4=""; //par
boolean	sleep_en = false;                   
float celsius, fahrenheit;
byte addr[8];
uint8_t temp_counter, cont_count=0;

/*
**********************************************************************************************
 *                GLOBAL VARIABLE DEFINITIONS
 **********************************************************************************************
 */

volatile uint8_t sensorcount = 0;  // Sensor ID 

/*  WDT TIMER          */
ISR(WDT_vect)
{
  sensorcount++;  
}

void setup(){

  Serial.begin(115200);
  xbee.setSerial(Serial);
  
  tempinit();
  //tx.reserve(80);
  s1.reserve(15); //Tc
  s2.reserve(15); //Tur
  s3.reserve(15); //UV
  s4.reserve(15); //PAR
  
  analogReference(INTERNAL);
  pinMode(sleep_pin, OUTPUT); 
  pinMode(UV_PIN, INPUT);
  pinMode(PAR_PIN, INPUT);
  pinMode(TUR_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);

}

void loop(){
  
  digitalWrite(sleep_pin, HIGH);
  sleep_en = true;
  while(sensorcount<15){
    wtdsleep();
    delay(200);    
  }
  
  digitalWrite(sleep_pin, LOW); // Disable radio sleep
  delay(5000); 
  
  char str[80];
  
  get_s1(); //temp
  
  get_s2(TUR_PIN);
  
  get_s3(UV_PIN); 
  
  get_s4(PAR_PIN);
  
  //String tx = SEP + NODE_ID + SEP + s1 + "," + s2 + "," + s3 + "#" + s4 + "#" + '\r' + '\n' ;
  String tx = "#" + s1 + "," + s2 + "," + s3 + "," + s4 + ',' + emsg + "#" + '\r' + '\n' ;
  tx.toCharArray(str,tx.length());
  ZBTxRequest ztx = ZBTxRequest(co, (uint8_t *)str, tx.length());
  xbee.send(ztx); delay(2000); 
  
  resetvars(); 
  
}

void resetvars(){
  
  s1="";
  s2="";
  s3="";
  s4="";
  //tx="";
  celsius=0;
  sleep_en = false;
  sensorcount=0;

}

void get_s2(int pin){

  char  input[10];  
  int rawSens = 0; 
  float milliVolts = 0;
  
  rawSens = analogRead(pin);
  milliVolts = ((float(rawSens) * 1100)/1023);
  dtostrf(milliVolts, 3, 2, input);
  s2 = input;
}

void get_s3(int pin){
  
  char  input[10];  
  int rawSens = 0; 
  float milliVolts = 0;
  
  rawSens = analogRead(pin);
  milliVolts = 5*(((float(rawSens) * 1100)/1023));
  dtostrf(milliVolts, 3, 2, input);
  s3 = input;
}

void get_s4(int pin){
  
  char  input[10];  
  int rawSens = 0; 
  float milliVolts = 0;
  
  rawSens = analogRead(pin);
  milliVolts = 5*(((float(rawSens) * 1100)/1023));
  dtostrf(milliVolts, 3, 2, input);
  s4 = input;
}

void tempinit(){

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return;
  }

}

void get_s1(){
  char  input[10];
  byte data[12];

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( byte i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];

  byte cfg = (data[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time

  celsius = (float)raw / 16.0;
  dtostrf(celsius, 3, 2, input);
  s1 = input;

}

void wtdsleep(){

  // Enabling radio sleep mode 
  //if(!sleep_en){   
   // digitalWrite(sleep_pin, HIGH);
   // sleep_en = true; 
 // }

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  MCUSR &= ~(1<<WDRF);

  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */

  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  /* Now enter sleep mode. */
  sleep_mode();

  /* The program will continue from here after the WDT timeout*/

  sleep_disable(); /* First thing to do is disable sleep. */

  /* Re-enable the peripherals. */
  power_all_enable();

  wdt_reset();
  MCUSR = 0;
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;

}

  
  
  
