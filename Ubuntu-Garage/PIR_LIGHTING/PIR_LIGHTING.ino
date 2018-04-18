/* Smart Bollard ECU Project:
 * By Amro Qandour
 */


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

/*  SENSOR ID                */
#define PIR   1
#define COMMS 2
#define FSR   3

/*  BRIGHTNESS LEVELS        */
#define BRIGHTNESS_PIR   100
#define BRIGHTNESS_COMM  200

/*  SLEEP TYPES              */
#define  SENSOR    1
#define  WATCHDOG  2

/*  TX ALARMS                */
#define PIR_TX  1

/*
**********************************************************************************************
 *                EXTERNAL INTERRUPTS
 **********************************************************************************************
 */

/*  INPUT PINS          */
const int PIR_PIN = 3;  //Interrupt Pin for PIR Sensor i.e. INT0    
const int COMM_PIN = 0; //Interrupt Pin for Rx Data i.e. INT2

/*  INTERRUPT NUMBER    */
const int PIR_INT = 0;
const int COMM_INT = 2;


/*
**********************************************************************************************
 *                DIGITAL OUTPUTS
 **********************************************************************************************
 */

const int LIGHT_PIN = 10            ; // LED Control Pin


/*
**********************************************************************************************
 *                GLOBAL VARIABLE DEFINITIONS
 **********************************************************************************************
 */

volatile uint8_t sensorMode = 0;  // Sensor ID 
/*
**********************************************************************************************
 *                EXTERNAL INTERUPT ROUTINES
 **********************************************************************************************
 */

/*  PIR SENSOR          */
void pirExtInt()       
{

  sensorMode = PIR; // PIR sensor triggered

}

/*  RX MESSAGE          */
void commExtInt(){
  
  /* Dummy ISR */   
}


/*  WDT TIMER          */
ISR(WDT_vect)
{
  /* Dummy ISR */ 
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
  // Config external interrupt pins
  pinMode(COMM_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);

  // Config output pins 
  pinMode(LIGHT_PIN, OUTPUT);


  // Config serial baud-rate 
  Serial1.begin(9600);    

}

/*
**********************************************************************************************
 **********************************************************************************************
 *                SLEEP FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void deepsleep()         // here we put the arduino to sleep
{

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

    // Enable external interrupts  
    attachInterrupt(PIR_INT, pirExtInt, RISING); 
    attachInterrupt(COMM_INT, commExtInt, LOW);                   

    sleep_mode();  // Put to sleep

    sleep_disable();  // Executed once woken from sleep       
    power_all_enable();
    // Disable external interrupts                         
    detachInterrupt(PIR_INT);
    detachInterrupt(COMM_INT);
    //digitalWrite(LIGHT_PIN, HIGH); // Set light brightness & switch light on 

}

void wtdsleep(){
    
    digitalWrite(LIGHT_PIN, HIGH);
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
    digitalWrite(LIGHT_PIN, LOW); 

}
  
  


/*
**********************************************************************************************
 **********************************************************************************************
 *                MAIN FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void loop()
{

  deepsleep(); 
  delay(200);

  if (sensorMode == PIR)
  {
    // TX sensor ID - only if ext_int is sensor & not RX msg
    Serial1.println(PIR_TX); 
    delay(500);   //  Send msg
  }
  wtdsleep();
  
  sensorMode = 0;
}


