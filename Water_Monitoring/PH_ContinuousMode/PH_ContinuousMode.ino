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
#include <Streaming.h>

/*
**********************************************************************************************
 *                LOCAL DEFINES
 **********************************************************************************************
 */

#define rxpin 2                                                                
#define txpin 3 

SoftwareSerial myserial(rxpin, txpin);
OneWire  ds(5);

String inputstring = "";                                                       //a string to hold incoming data from the PC
String sensorstring = "";                                                      //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                                          //have we received all the data from the PC
boolean sensor_stringcomplete = false;                                         //have we received all the data from the Atlas Scientific product

float celsius, fahrenheit;
byte addr[8];
int cont_count=0;
uint8_t temp_counter=0; 
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
}


/*
**********************************************************************************************
 **********************************************************************************************
 *                INITIALISATION FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void setup()
{

  // Config serial baud-rate 
  Serial.begin(9600);
  myserial.begin(38400);  
  
  tempinit();
  sensorstring.reserve(30);
  
  // pH chip initialisation
  myserial.print("e"); delay(100); myserial.print('\r'); delay(100);


}

/*
**********************************************************************************************
 **********************************************************************************************
 *                SLEEP FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void wtdsleep(){

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
  // Sleep for 2 mins
  if(sensorcount!=15){
    wtdsleep();
    delay(200);    
  }

  else{
    delay(200);
    sensorcount = 0;
    celsius = 0;
    tempread();

    // Take a single pH reading
    myserial.print(celsius); delay(100); myserial.print('\r'); delay(100);

    // Read software serial port for incoming pH data
    while (myserial.available()) {
      char inchar = (char)myserial.read();
      sensorstring+=inchar;
      // Transmit data when <CR> is received           
      if (inchar == '\r') { 
        Serial << "P:" << sensorstring;                              
        cont_count++;  
        sensorstring = "";  
      }  
    }
    // Disable continous mode for the pH chip
    //myserial.print("e"); delay(100); myserial.print('\r'); delay(100); 
  }

}


