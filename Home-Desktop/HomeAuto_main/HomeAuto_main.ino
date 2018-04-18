/*
        
    YELLOW ------- GREEN
    
            RED
            
     WHITE --------- BLUE       
  //BLUE LED FOR ERROR. 
 
  //YELLOW LED ON: SYSTEM_DOWN (MUST SAY PASSWORD)
  //YELLOW LED OFF: SYSTEM_ON (AFTER SAYING PASSWORD) 

*/


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #include "SoftwareSerial.h"
  SoftwareSerial port(12,13);
#else // Arduino 0022 - use modified NewSoftSerial
  #include "WProgram.h"
  #include "NewSoftSerial.h"
  NewSoftSerial port(12,13);
#endif

#include "EasyVR.h"
#include "SoftwareSerial.h"

SoftwareSerial Xbee(12,13);
EasyVR easyvr(Serial);

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
  GROUP_2  = 2,
  GROUP_16 = 16,
};

enum Group0 
{
  G0_START = 0,
};

enum Group1 
{
  G1_SWITCH = 0,
  G1_SYSTEM_DOWN = 1,
  G1_START_PUMP = 2,
  G1_PUMP_STOP = 3,
  G1_LIGHTS = 4,
  G1_LIGHTS_OFF = 5,
};

enum Group2 
{
  G2_SWITCH = 0,
  G2_SYSTEM_DOWN = 1,
  G2_LIGHTS = 2,
  G2_LIGHTS_OFF = 3,
  G2_MAKE_COFFEE = 4,
  G2_COFFEE_OFF = 5,
};

enum Group16
{
  G16_SYSTEM_ON = 0,
};


EasyVRBridge bridge;

int8_t group, idx, level;

void setup()
{
  /*
  // bridge mode?
  if (bridge.check())
  {
    cli();
    bridge.loop(0, 1, 12, 13);
  }
  */
  
  // run normally
  pinMode(2,OUTPUT); //RIGHT SSR
  pinMode(3,OUTPUT); // LEFT SSR
  
  pinMode(4,OUTPUT); //GREEN
  pinMode(5,OUTPUT); // WHITE
  pinMode(6,OUTPUT); //RED
  pinMode(7,OUTPUT); //YELLOW
  pinMode(8,OUTPUT); //BLUE
  
  Serial.begin(9600); 
  //port.begin(9600); 
  
  if (!easyvr.detect())
  {
    for (;;);
  }
  Xbee.begin(9600); delay(1000);
  
  digitalWrite(6, HIGH); // RED - POWER ON
  digitalWrite(7, HIGH); 
  easyvr.setTimeout(31);
  easyvr.setLanguage(0);
  level = EasyVR:: HARDEST;
  group = EasyVR::PASSWORD;  
  easyvr.playSound(3, EasyVR::VOL_FULL); delay(200); 
  easyvr.playSound(5, EasyVR::VOL_FULL);
  
  
}

void action();

void loop(){
  
  
  digitalWrite(4, HIGH); // GREEN DETECTION ON
  
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
  
  digitalWrite(4, LOW); // GREEN DETECTION OFF

  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    //Serial.print("Command: ");
    //Serial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      digitalWrite(8, HIGH); // BLUE ANALYZING
    }
      //Serial.println();
    easyvr.playSound(0, EasyVR::VOL_FULL);
    digitalWrite(8, LOW); // BLUE FINISHED ANALYZING
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout()){
      digitalWrite(8, HIGH); delay(200); 
      digitalWrite(8, LOW); 
    }
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      digitalWrite(8, HIGH);
      delay(200); 
      digitalWrite(8, LOW);
    }
  }
}

void action()
{
    switch (group)
    {
    //START GROUP 0  
    case GROUP_0:
      switch (idx)
      {
      case G0_START:
        // write your action code here
         //group = 1
        break;
      }
      break;
      //End GROUP 0.
    
    //START GROUP_1  
    case GROUP_1:
      switch (idx)
      {
      case G1_SWITCH:
          group = GROUP_2;
          digitalWrite(5, LOW);
          digitalWrite(7, HIGH);
        break;
        
      case G1_SYSTEM_DOWN:
          digitalWrite(5, HIGH);
          digitalWrite(7, LOW);
          group = 16;
        break;
        
      case G1_START_PUMP:
          digitalWrite(3, HIGH);
        break;
        
      case G1_PUMP_STOP:
          digitalWrite(3, LOW);
        break;
        
      case G1_LIGHTS:
          digitalWrite(2, HIGH);
        break;
        
      case G1_LIGHTS_OFF:
          digitalWrite(2, LOW);
        break;
      }
      break;
      //END GROUP_1
      
    //START GROUP_2  
    case GROUP_2:
      switch (idx)
      {
      case G2_SWITCH:
          group = GROUP_1;
          digitalWrite(5, HIGH);
          digitalWrite(7, LOW); 
        break;
        
      case G2_SYSTEM_DOWN:
          digitalWrite(5, HIGH);
          digitalWrite(7, LOW); 
          group = 16;  
        break;
        
      case G2_LIGHTS:
          Xbee.print("LIGHTS");
          //easyvr.playSound(0, EasyVR::VOL_FULL);
        break;
        
      case G2_LIGHTS_OFF:
          Xbee.print("LIGHTS_OFF");
          //easyvr.playSound(0, EasyVR::VOL_FULL);
        break;
        
      case G2_MAKE_COFFEE:
          Xbee.print("COFFEE");
          //easyvr.playSound(0, EasyVR::VOL_FULL);
        break;
        
      case G2_COFFEE_OFF:
          Xbee.print("COFFEE_OFF");
          //easyvr.playSound(0, EasyVR::VOL_FULL);
        break;
      }
      break;
      //END GROUP_2
      
    //START GROUP_16  
    case GROUP_16:
      switch (idx)
      {
      case G16_SYSTEM_ON:
        // write your action code here
         easyvr.playSound(2, EasyVR::VOL_FULL);
         //digitalWrite(5, LOW);
         //digitalWrite(7, HIGH);
         group = 1; 
        break;
      }
      break;
      //END GROUP 16
    }
}
