#include <LiquidCrystal.h>
#include <SoftwareSerial.h> 
// initialize the library with the numbers of the interface pins

#define NEW_GREEN_DELAY  20
#define INITIAL_GREEN_DELAY  200

int ledPinRed =  11;
int ledPinGreen = 12;



int counter = 0; 
unsigned int greenDelay = INITIAL_GREEN_DELAY;
boolean redLED_isOn = false;
boolean greenLED_isOn = false;
boolean c_timer = false; 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
boolean sensors = false;
SoftwareSerial mySerial(8,10); 
 int ctimer = 0;
//Forward declaration, to let redLED call to resumeTask(greenLED). It's not necessary if we put the greenLED 
//itself here, but declareLoopTask may be usefull in more complex situations:


declareTaskLoop(greenLED);


taskLoop(redLED)
{
  static unsigned char counter = 0;
  
  if (!greenLED_isOn)
  {
     if (counter >2)
       resumeTask(greenLED);
     counter++;
  }
  
  redLED_isOn = false;
  delay(1000);
  redLED_isOn = true; 
  delay(1000);
}


taskLoop(greenLED)
{
  static unsigned char counter = 1;

  digitalWrite(ledPinGreen, HIGH);  // set the LED on
  delay(greenDelay);
  digitalWrite(ledPinGreen, LOW);  // set the LED off
  delay(greenDelay);

  if ( (counter >= 9) && (greenDelay != NEW_GREEN_DELAY) )
    greenDelay = NEW_GREEN_DELAY; //now, after 10 blinks, accelerates
  if (counter >= 99)
  {
    //Reset vars, so next time, if the task is resumed, it executes all again:
    counter = 0;
    greenDelay = INITIAL_GREEN_DELAY;
    greenLED_isOn = false;
    suspend();      //After a while, the tasks suspends itself (forever)
  }
  counter++;
}

taskLoop(RFID)
{
    if(mySerial.available()){
      sensors = true;
      char tagString[13];
      int index = 0;
      boolean reading = false;
      
      while(mySerial.available() > 0){
  
        int readByte = mySerial.read(); //read next available byte
    
        if(readByte == 2) reading = true; //begining of tag
        if(readByte == 3) reading = false; //end of tag
    
        if(reading && readByte != 2 && readByte != 10 && readByte != 13){
          //store the tag
          tagString[index] = readByte;
          index ++;
          lcd.print((char)readByte);   
        }
      }
         
    }

  vTaskDelay(2000);   
  lcd.clear();  
   
  //clearTag(tagString); //Clear the char of all value

}

/*
taskLoop(initSensor)
{
  
  while(ctimer==0){
    
    digitalWrite(11, HIGH); 
    
    delay(50);

    ctimer = 1; 
  }
  
  
  
}

  

taskLoop(readSensor)
{
  
 while(ctimer==1){
   
   digitalWrite(11, LOW);
   
   delay(50);
   
   ctimer = 0;
 }
 
}


taskLoop(act)
{
  
  while(ctimer == 1){
    
    digitalWrite(12, HIGH);
  }
  
  while(ctimer == 0){
    
    digitalWrite(11, LOW);
  }
  
}
*/  
  

// The setup() method runs once, when the sketch starts

void setup()   
{                
  // Initialize the digital pins as outputs:
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  lcd.begin(16, 2);
  mySerial.begin(9600); 
//  Serial.begin(9600); 
  
  
  createTaskLoop(RFID, NORMAL_PRIORITY); 
  createTaskLoop(redLED, NORMAL_PRIORITY);
  createTaskLoop(greenLED, LOW_PRIORITY);
//  createTaskLoop(act, NORMAL_PRIORITY)
//  createTaskLoop(initSensor, NORMAL_PRIORITY); 
//  createTaskLoop(readSensor, NORMAL_PRIORITY);
  //createTaskLoop(Sensors, LOW_PRIORITY); 
  //This initializes the main loop's with a different priority (default is LOW_PRIORITY):
  //initMainLoopPriority(NORMAL_PRIORITY);

  //Try this and see what happends:
  //suspendTask(redLED);
}


// This is the main loop() method, wich runs over and over again,
// as long as the Arduino has power. Is a LOW_PRIORITY taskLoop:

void loop()                     
{ 
  

  
    
 
   
  if (redLED_isOn)
  {
    digitalWrite(ledPinRed, LOW);  // set the LED off
    delay(25);                      // The OS can be tested reducing these delays, and seeing how both LEDs work together...
    digitalWrite(ledPinRed, HIGH);   // set the LED on
    delay(25);    
  }
  else
  {
    digitalWrite(ledPinRed, LOW);  //  LED is off
    //If nextTask is not called, the application will not hang, because the OS is preemptive. BUT, the current task
    //will consume a lot of computational resources (due to it's lack of a delay() in this branch), the application will 
    //turn slower, and the other tasks may be affected by this, loossing precision in their timing:
    
    nextTask();
  }
  
  
  
}

void clearTag(char one[]){
///////////////////////////////////
//clear the char array by filling with null - ASCII 0
//Will think same tag has been read otherwise
///////////////////////////////////
  for(int i = 0; i < strlen(one); i++){
    one[i] = 0;
  }
}
