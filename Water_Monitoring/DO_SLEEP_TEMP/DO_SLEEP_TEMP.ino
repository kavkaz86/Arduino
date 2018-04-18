/*
**********************************************************************************************
*                INCLUDE FILES
**********************************************************************************************
*/
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include <OneWire.h>

/*
**********************************************************************************************
*                GLOBAL DEFINITINOS
**********************************************************************************************
*/

#define  RX_1  "r"
#define  CONDUCTIVITY  0
#define  COUNT_MAX  15
#define  DS18S20_ID 0x10
#define  DS18B20_ID 0x28


/*
**********************************************************************************************
*                GLOBAL VARIABLE DEFINITIONS
**********************************************************************************************
*/

int  sleepCount  =  0; 
String  sensorStringdo   =  "";
boolean  do_string  =  false;
char  dobuffer[10];
float  temp  =  0; 

SoftwareSerial doport(4,5);
OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)


/*
**********************************************************************************************
*                EXTERNAL INTERUPT ROUTINES
**********************************************************************************************
*/

/*  WDT TIMER          */
ISR(WDT_vect)
{
} 


void setup()
{
  Serial.begin(9600);      // sets the serial port to 9600
  
  doport.begin(38400);
}

/*
**********************************************************************************************
 **********************************************************************************************
 *                SLEEP FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void sleepNow()         // here we put the arduino to sleep
{

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

void loop()
{
  
  /*
  while(sleepCount <  COUNT_MAX){
   delay(200); 
   sleepNow();
   delay(200);
   sleepCount++;   
  }
  */
  
  
  sleepNow(); 
  delay(200);
  
  if(getTemperature() == true){
  
    doport.print(temp); doport.print(","); doport.print(CONDUCTIVITY); doport.print("\r");
    delay(200); 
  }
      
  doport.print(RX_1); doport.print("\r");     
  doport.listen();
  while(doport.available()){
    
    sensorStringdo += (char)doport.read();
  
  }
  
  sensorStringdo.toCharArray(dobuffer, 6); 
  String output = sensorStringdo ;
    
  Serial.println(output);
  sensorStringdo = "";
 
  sleepCount = 0 ; 
  
  delay(200); 
}


boolean getTemperature(){
 
   byte i;
   byte present = 0;
   byte data[12];
   byte addr[8];
   //find a device
   if (!ds.search(addr)) {
   ds.reset_search();
   return false;
   }
   if (OneWire::crc8( addr, 7) != addr[7]) {
   return false;
   }
   if (addr[0] != DS18S20_ID && addr[0] != DS18B20_ID) {
   return false;
   }
   ds.reset();
   ds.select(addr);
   // Start conversion
   ds.write(0x44, 1);
   // Wait some time...
   delay(850);
   present = ds.reset();
   ds.select(addr);
   // Issue Read scratchpad command
   ds.write(0xBE);
   // Receive 9 bytes
   for ( i = 0; i < 9; i++) {
   data[i] = ds.read();
   }
   // Calculate temperature value
   temp = ( (data[1] << 8) + data[0] )*0.0625;
   return true;
}
