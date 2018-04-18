#include "Streaming.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

static const byte PIN_HEATER   = 8;
static const byte PIN_LED   = 5;
static const byte PIN_CO   = A0;

uint8_t sleep_ct = 0;
int iter = 0 ;

bool gas_detect = false;

/*  WDT TIMER          */
ISR(WDT_vect)
{
}

void setup() {
  pinMode(PIN_HEATER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_CO, INPUT);

  Serial.begin(9600);
  //digitalWrite(PIN_HEATER, LOW);

}

void sleepNow(void)         // here we put the arduino to sleep
{

  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
 // sleep_enable();
 // MCUSR &= ~(1 << WDRF);

  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  //WDTCSR |= (1 << WDCE) | (1 << WDE);

  /* set new watchdog timeout prescaler value */
  //WDTCSR = 1 << WDP0 | 1 << WDP3; /* 8.0 seconds */
 set_sleep_mode(SLEEP_MODE_PWR_DOWN);             // select the watchdog timer mode
 MCUSR &= ~(1 << WDRF);                           // reset status flag
 WDTCSR |= (1 << WDCE) | (1 << WDE);              // enable configuration changes
 WDTCSR = (1<< WDP0) | (1 << WDP1) | (1 << WDP2); // set the prescalar = 7
 WDTCSR |= (1 << WDIE);                           // enable interrupt mode
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  sleep_enable();                                  // enable the sleep mode ready for use
 sleep_mode();       
  /* Now enter sleep mode. */
  //sleep_mode();

  /* The program will continue from here after the WDT timeout*/

  sleep_disable(); /* First thing to do is disable sleep. */

  /* Re-enable the peripherals. */
  power_all_enable();

  wdt_reset();
  MCUSR = 0;
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;

}

void loop() {
  
  
  if(gas_detect){
    gas_detect = false; 
    digitalWrite(PIN_LED, LOW);
  }
 
  digitalWrite(PIN_HEATER, HIGH);
  //delay(60000);
   while (sleep_ct < 30) {

    sleepNow();
    delay(200);
    sleep_ct++;
  }
   sleep_ct = 0;
  
  int Co_val = analogRead(PIN_CO);
  digitalWrite(PIN_HEATER, LOW);
  float Co_v = 5.0 * Co_val / 1024.0 ;
  
  if (Co_v > 3.00) {
    digitalWrite(PIN_LED, HIGH);
    gas_detect = true;
  }
  
  Serial << iter << " | CO (adc) = " << Co_val << " | CO (V)= " << Co_v << endl; delay(1000);
  
  iter++;
  
  
  while (sleep_ct < 30) {

    sleepNow();
    delay(200);
    sleep_ct++;
  }
   sleep_ct = 0;
  

}
