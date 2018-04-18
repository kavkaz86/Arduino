/* Smart Ballard ECU Project: 
 * By Amro Qandour
 */ 

#include <avr/sleep.h>


#define pir 1

int pirPin = 2; //Interrupt Pin for PIR Sensor i.e. INT1       


int lightPin = 13; //LED Control Pin
int sleepStatus = 0;             
int count = 0;                   
volatile uint8_t sensorMode=0;


void pirWakeUp()        
{
  sensorMode = pir;

}


void setup()
{

  pinMode(pirPin, INPUT);
 // digitalWrite(pirPin, HIGH); 
  pinMode(lightPin, OUTPUT); 

  /* Now it is time to enable an interrupt. In the function call 
   * attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.   
   * 
   * B   Name of a function you want to execute while in interrupt A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level trigger
   *             CHANGE     a change in level trigger
   *             RISING     a rising edge of a level trigger
   *             FALLING    a falling edge of a level trigger
   *
   * In all but the IDLE sleep modes only LOW can be used.
   */

  //attachInterrupt(0, pirWakeUp, LOW);                                            
                                      
  attachInterrupt(0, pirWakeUp, RISING);      
}

void sleepNow()         // here we put the arduino to sleep
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and 
     * wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we 
     * choose the according 
     * sleep mode: SLEEP_MODE_PWR_DOWN
     * 
     */  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 

    /* Now it is time to enable an interrupt. We do it here so an 
     * accidentally pushed interrupt button doesn't interrupt 
     * our running program. if you want to be able to run 
     * interrupt code besides the sleep function, place it in 
     * setup() for example.
     * 
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.   
     * 
     * B   Name of a function you want to execute at interrupt for A.
     *
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     *
     * In all but the IDLE sleep modes only LOW can be used.
     */
   
    attachInterrupt(0, pirWakeUp, RISING);      


    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the 
                             // wakeUpNow code will not be executed 
                             // during normal running time.
    
}

void loop()
{
  digitalWrite(lightPin, LOW);  
  sleepNow(); delay(200); 
  
  
  switch(sensorMode){
  
    case pir:
      sensorMode=0;
      digitalWrite(lightPin, HIGH);
      while(digitalRead(2) == HIGH);   
      //delay(2000);
      break;
            
    default:
      break;
  }

}

