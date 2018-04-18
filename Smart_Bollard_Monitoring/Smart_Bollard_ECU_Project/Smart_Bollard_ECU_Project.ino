/* Smart Bollard ECU Project:
 * By Amro Qandour
 */
 
 
/*
**********************************************************************************************
*                INCLUDE FILES
**********************************************************************************************
*/

#include <avr/sleep.h>


/*
**********************************************************************************************
*                LOCAL DEFINES
**********************************************************************************************
*/

/*  SENSOR ID                */
#define PIR   1
//#define fsr 2
#define COMMS 3

/*  BRIGHTNESS LEVELS        */
#define BRIGHTNESS_PIR   100
#define BRIGHTNESS_COMM  200
#define brightness3      300


/*
**********************************************************************************************
*                EXTERNAL INTERRUPTS
**********************************************************************************************
*/

/*  INPUT PINS          */
const int PIR_PIN = 3;  //Interrupt Pin for PIR Sensor i.e. INT0    
//int fsrPin = 2;       //Interrupt Pin for FSR sensor i.e. INT1
const int COMM_PIN = 0; //Interrupt Pin for Rx Data i.e. INT2

/*  INTERRUPT NUMBER    */
const int PIR_INT = 0;
const int COMM_INT = 2;


/*
**********************************************************************************************
*                DIGITAL OUTPUTS
**********************************************************************************************
*/

const int LIGHT_PIN = 9; // LED Control Pin


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
  analogWrite(LIGHT_PIN, BRIGHTNESS_PIR); // Set light brightness & switch light on   
}

/*  RX MESSAGE          */
void commExtInt(){
  sensorMode = COMMS; // Msg RX
  analogWrite(LIGHT_PIN, BRIGHTNESS_COMM); // Set light brightness & switch light on 
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
  
  // Enable external interrupts  
  attachInterrupt(PIR_INT, pirExtInt, RISING);     
  attachInterrupt(COMM_INT, commExtInt, LOW);

  sleep_mode();  // Put to sleep
  sleep_disable();  // Executed once woken from sleep       
  
  // Disable external interrupts                         
  detachInterrupt(PIR_INT);     
  detachInterrupt(COMM_INT);
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
//  digitalWrite(lightPin, LOW); // Switch light off
//  delay(200); 
//  sleepNow(); // Enter sleep mode
//  delay(200);
// 
//  switch(sensorMode){
//      
//    case pir:
//      sensorMode=0;
//      Serial1.println("Presence Detected");  //  Send msg  
//      analogWrite(lightPin, brightness1);    //  Set light brightness & switch light on 
//      //digitalWrite(9, HIGH);                 //  Turn on LED
//      delay(10000);                          //  10 sec delay
//      break;
//    
//    
//    case comms:
//      sensorMode=0;
//      analogWrite(lightPin, brightness2);    //  Set light brightness & switch light on
//      //digitalWrite(9, HIGH);                 //  Turn on LED
//      delay(10000);                          //  10 sec delay
//      break;
//   
////    case fsr:
////      sensorMode=0;
////      Serial1.println("FSR Detected");
////      digitalWrite(9, HIGH); 
////      analogWrite(lightPin, brightness3);
////      delay(10000);
////      break;
//           
//    default:
//      break;
//  }
  
  /*
  **********************
  *  NEW CODE
  **********************
  */
  
  // Turn off LED
  digitalWrite(LIGHT_PIN, LOW); // Switch light off
  delay(200); 
  
  // Enter deep sleep - wake up triggered by external interupt
  sleepNow(); // Enter sleep mode
  delay(200);
  
  if (sensorMode == PIR)
  {
    // TX sensor ID - only if ext_int is sensor & not RX msg
    Serial1.println("Presence Detected");  //  Send msg
  }
  
  sensorMode = 0;
    
  // Enter low sleep - wake up triggered by timer
  delay(10000); // NOTE: Need to replace delay with WDT sleep
 
}
