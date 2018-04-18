/*
**********************************************************************************************
 *                INCLUDE FILES
 **********************************************************************************************
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>                                                 
#include <OneWire.h>
#include "Streaming.h"
#include "Xbee.h"

/*
**********************************************************************************************
 *                LOCAL DEFINES
 **********************************************************************************************
 */

/*  SENSOR UNITS           */
#define  UV_DEL       "C:"
#define  PAR_DEL      "D:"
#define  TUR_DEL      "H:"
#define  TEMP_DEL     "A:"
#define  END_OF_TX    '\r'
#define  sleep_pin    3 
#define  TX_DELAY     100
#define  SENSE_DELAY  500
/*  INPUT PINS            */
const int UV_PIN = A1;  //UV SENSOR ANALOG PIN = A1  
const int PAR_PIN =A2;  //PAR SENSOR ANALOG PIN = A2 
const int TUR_PIN = A3; //TURBIDITY SENSOR ANALOG PIN = A3 
const int TEMP_PIN = 2; //TEMPERATURE SENSOR DIGITAL PIN = 9 
OneWire  ds(2);

String inputstring = "";         
String sensorstring = "";                                                      
boolean	sleep_en = false;                   
float celsius, fahrenheit;
byte addr[8];
uint8_t temp_counter, cont_count=0;

/*
**********************************************************************************************
 *                EXTERNAL INTERRUPTS
 **********************************************************************************************
 */



/*
**********************************************************************************************
 *                DIGITAL OUTPUTS
 **********************************************************************************************
 */
XBee xbee = XBee();
XBeeAddress64  co = XBeeAddress64(0x00000000, 0x00000000);

/*
**********************************************************************************************
 *                GLOBAL VARIABLE DEFINITIONS
 **********************************************************************************************
 */

volatile uint8_t sensorcount = 0;  // Sensor ID 
/*
**********************************************************************************************
 *                EXTERNAL INTERUPT ROUTINES
 **********************************************************************************************
 */


/*  WDT TIMER          */
ISR(WDT_vect)
{
  sensorcount++;  
}


/*
**********************************************************************************************
 **********************************************************************************************
 *                Temperature Functions
 **********************************************************************************************
 **********************************************************************************************
 */

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

void tempread(){
  char  input[10];
  byte data[12];
  sensorstring=TEMP_DEL;

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
  
  sensorstring+=input;
  sensorstring+=END_OF_TX; 
  
  Serial << sensorstring; 
  delay(TX_DELAY); 
  
  sensorstring="";
  celsius=0;

}


/*
**********************************************************************************************
 **********************************************************************************************
 *                INITIALISATION FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void setup(){

  Serial.begin(115200);
  
  tempinit();
  sensorstring.reserve(30);
  analogReference(INTERNAL);
  pinMode(sleep_pin, OUTPUT); 
  pinMode(UV_PIN, INPUT);
  pinMode(PAR_PIN, INPUT);
  pinMode(TUR_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);

}

/*
**********************************************************************************************
 **********************************************************************************************
 *                SLEEP FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void wtdsleep(){

  // Enabling radio sleep mode 
  if(!sleep_en){   
    digitalWrite(sleep_pin, HIGH);
    sleep_en = true; 
  }

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

/*
**********************************************************************************************
 **********************************************************************************************
 *                UV + PAR SENSOR FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void sensor(int pin){
  char  input[10];  
  int rawSens = 0; 
  float milliVolts = 0; 

  if(pin==UV_PIN){
    sensorstring=UV_DEL;
  }
  else if(pin==PAR_PIN){  
    sensorstring=PAR_DEL; 
  }
  else if(pin==TUR_PIN){
    sensorstring=TUR_DEL;
  }

  rawSens = analogRead(pin);
  milliVolts = ((float(rawSens) * 1100)/1023);

  if(pin==UV_PIN||PAR_PIN){
    milliVolts = milliVolts * 5;
  }

  dtostrf(milliVolts, 3, 2, input);
  sensorstring+=input;
  sensorstring+=END_OF_TX;


  Serial << sensorstring; 
  delay(TX_DELAY); 

  sensorstring=""; 
}

void loop(){
  // Sleep for 2 mins
  while(sensorcount<15){
    wtdsleep();
    delay(200);    
  }

  // Node wakes up after two minutes
  digitalWrite(sleep_pin, LOW); // Disable radio sleep
  delay(15000); // 15-seconds required for radio reassociation 
  
  sensor(UV_PIN); 
  delay(SENSE_DELAY); 
  sensor(PAR_PIN);
  delay(SENSE_DELAY); 
  sensor(TUR_PIN);
  delay(SENSE_DELAY);
  tempread();
  delay(SENSE_DELAY); 
  
  sleep_en = false;
  sensorcount=0;


}


