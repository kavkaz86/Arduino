/* Smart Ballard ECU Project: 
 * By Amro Qandour
 */ 

#include <avr/sleep.h>


#define vibration 1
#define pir 2
#define comms 3
#define fsr 4
#define brightness1 50
#define brightness2 100
#define brightness3 200
#define brightness4 300

//int commPin = 0; //Interrupt Pin for Rx Data
int pirPin = 3; //Interrupt Pin for PIR Sensor i.e. INT1       
//int vibPin = 3; //Interrupt Pin for Vibration Sensor i.e. INT0
int fsrPin = 2; //Interrupt Pin for FSR sensor

int lightPin = 9; //LED Control Pin
int sleepStatus = 0;             
int count = 0;                   
volatile uint8_t sensorMode=0;

/*void vibWakeUp()        
{
  sensorMode=1;
} */

void pirWakeUp()        
{
  sensorMode=2;
}

void commWakeUp(){
  
  sensorMode=3; 
}

void fsrWakeUp(){
  
  sensorMode=4;
}




void setup()
{
  
  //pinMode(vibPin, INPUT);
  //pinMode(commPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(fsrPin, INPUT);  
  pinMode(lightPin, OUTPUT); 
  //pinMode(13, OUTPUT); 
  //Serial.begin(9600);
  Serial1.begin(9600); 

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
                                      
  attachInterrupt(0, pirWakeUp, FALLING);      

  attachInterrupt(1, fsrWakeUp, RISING);  
  
  attachInterrupt(2, commWakeUp, FALLING); 
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
     
    //attachInterrupt(0, pirWakeUp, LOW);                                            
                                      
    //attachInterrupt(1, vibWakeUp, LOW);      

    //attachInterrupt(2, commWakeUp, LOW);  
  
   // attachInterrupt(4, fsrWakeUp, LOW);   
   
    attachInterrupt(0, pirWakeUp, FALLING);      

    attachInterrupt(1, fsrWakeUp, RISING); 
    
    attachInterrupt(2, commWakeUp, FALLING); 

    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the 
                             // wakeUpNow code will not be executed 
                             // during normal running time.
    
    detachInterrupt(1);
    
    detachInterrupt(2);
    
   // detachInterrupt(4);
    
}

void loop()
{
  digitalWrite(lightPin, LOW);  
  //digitalWrite(13, LOW); 
  delay(200); 
  sleepNow(); delay(200); 
  
  switch(sensorMode){
    
    /*case vibration:
      sensorMode=0;
      Serial1.println("Vibration Detected");
      digitalWrite(9, HIGH);  
      //digitalWrite(13, HIGH);
      analogWrite(lightPin, brightness1); 
      delay(1000);
      break; */
      
    
    case pir:
      sensorMode=0;
      Serial1.println("Presence Detected"); 
      analogWrite(lightPin, brightness2); 
      digitalWrite(9, HIGH);  
      //digitalWrite(13, HIGH);
      delay(1000);
      break;
     
     
    case comms:
      sensorMode=0;
      analogWrite(lightPin, brightness3); 
      digitalWrite(9, HIGH);  
      //digitalWrite(13, HIGH);
      if (Serial1.available()){
        delay(100); 
        while(Serial1.available() > 0){
          Serial.print((char)Serial1.read()); 
        }
      }
      delay(1000); 
      break;
    
    case fsr:
      sensorMode=0;
      Serial1.println("FSR Detected");
      digitalWrite(9, HIGH);  
      //digitalWrite(13, HIGH);
      analogWrite(lightPin, brightness4); 
      delay(1000);
      break; 
            
    default:
      break;
  }
  

  
   /*if (Serial.available()) {
    int val = (char)Serial.read();
    if (val == 'S') {
      Serial.println("Serial: Entering Sleep mode");
      delay(100);
      sleepNow();     // sleep function called here
    }
    if (val == 'A') {
      Serial.println("Serial Triggered Interrupt"); // classic dummy message
    }
  } */
  
 
 
}

