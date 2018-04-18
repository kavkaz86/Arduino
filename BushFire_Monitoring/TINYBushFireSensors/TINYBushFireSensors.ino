/*
**********************************************************************************************
*                INCLUDE FILES
**********************************************************************************************
*/
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

/*
**********************************************************************************************
*                LOCAL DEFINES
**********************************************************************************************
*/

#define  SENSOR_ID  'A'
#define  DELIMITER    ':' 
#define  END_OF_TX    '\r'

/*
**********************************************************************************************
*                ANALOG INPUTS
**********************************************************************************************
*/

const int SMOKE_SENSOR = A0; // LED Control Pin


/*
**********************************************************************************************
*                GLOBAL VARIABLE DEFINITIONS
**********************************************************************************************
*/

float sensorValue;
String tx = "";


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
  tx.reserve(30);
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
  sensorValue = 5.0*analogRead(SMOKE_SENSOR)/1023;       // read analog input pin 0
  //Serial.println(sensorValue);  // prints the value read
  tx_msg(SENSOR_ID, sensorValue);
  delay(200);                        // wait 100ms for next reading
  
  sleepNow();
  delay(200);
  sensorValue = 0;  
}

void  tx_msg(char  id,  float  sensor_value){
  
  char  input[10];
  dtostrf(sensor_value, 3, 2, input);
  tx += id;
  tx += DELIMITER; 
  tx += input;
  tx += END_OF_TX; 
  Serial.print(tx);
  delay(1000);
  tx = "";  
}

