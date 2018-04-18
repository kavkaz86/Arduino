#include "EasyVR.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

//SoftwareSerial port(12,13);
SoftwareSerial xbee(2,3); 
EasyVR easyvr(Serial);


//LED Definitions: 
#define BLUE  4
#define RED  5
#define GREEN  6
#define YELLOW  7
#define WHITE  8


//SSR Definitions: 
#define  SSR_1  9
#define  SSR_2  10

//XBee Commands:
#define  radio_on_cmd  "1"
#define  radio_off_cmd  "2"
#define  fan_on_cmd  "3"
#define  fan_off_cmd  "4"

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
  GROUP_16 = 16,
};

enum Group0 
{
  G0_COMPUTER = 0,
};

enum Group1 
{
  G1_LIGHT_ON = 0,
  G1_LIGHT_OFF = 1,
  G1_LOCK_DOOR = 2,
  G1_UNLOCK_DOOR = 3,
  G1_RADIO_ON = 4,
  G1_RADIO_OFF = 5,
  G1_FAN_ON = 6,
  G1_FAN_OFF = 7,
  G1_SHUTDOWN = 8,
};

enum Group16
{
  G16_COMPUTER = 0,
};


EasyVRBridge bridge;

int8_t group, idx;

void setup()
{
  
  for(int i=4; i<11; i++){
    pinMode(i, OUTPUT); 
  }
  
  // bridge mode?
  //if (bridge.check())
  //{
   // cli();
   // bridge.loop(0, 1, 12, 13);
  //}
  // run normally
  Serial.begin(9600);
  //port.begin(9600);
  xbee.begin(9600); 

  if (!easyvr.detect())
  {
  //  Serial.println("EasyVR not detected!");
    for (;;);
  }

  //easyvr.setPinOutput(EasyVR::IO1, LOW);
  //Serial.println("EasyVR detected!");
  easyvr.setTimeout(15);
  easyvr.setLanguage(EasyVR::ENGLISH);
  easyvr.playSound(3, EasyVR::VOL_FULL);
  group = EasyVR::TRIGGER;
  digitalWrite(RED, HIGH);
}

void action();

void loop()
{
  //easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)
  digitalWrite(BLUE, HIGH); 
  //Serial.print("Say a command in Group ");
  //Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
  
  digitalWrite(BLUE, LOW); 
  //easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off
  
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
   // Serial.print("Command: ");
   // Serial.print(idx);
    easyvr.dumpCommand(group, idx, name, train);
      

    
     // Serial.print(" = ");
     // Serial.println(name);
    
    
      //Serial.println();
    //easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
    
  }
  else // errors or timeout
  {
    
    if (easyvr.isTimeout()) delay(1);
     
    //  Serial.println("Timed out, try again...");
      if(easyvr.getError() > 0){
       easyvr.playSound(1, EasyVR::VOL_FULL);
       digitalWrite(YELLOW, HIGH);
       delay(2000);
       digitalWrite(YELLOW, LOW);
       easyvr.playSound(4, EasyVR::VOL_FULL);
    }    
  }
}

void action()
{
    switch (group)
    {
    case GROUP_0:
      switch (idx)
      {
      case G0_COMPUTER:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        digitalWrite(WHITE, HIGH);
        easyvr.playSound(2, EasyVR::VOL_FULL); 
        group=GROUP_1;
        
        break;
      }
      break;
    case GROUP_1:
      switch (idx)
      {
      case G1_LIGHT_ON:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        digitalWrite(SSR_1, HIGH);
        delay(200);         
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW); 
        break;
        
      case G1_LIGHT_OFF:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        digitalWrite(SSR_1, LOW);
        delay(200);
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW);  
        break;
        
      case G1_LOCK_DOOR:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        digitalWrite(SSR_2, HIGH);
        delay(200);        
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW); 
        break;
        
      case G1_UNLOCK_DOOR:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        digitalWrite(SSR_2, LOW);
        delay(200);
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW); 
        break;
        
      case G1_RADIO_ON:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        xbee.println(radio_on_cmd);
        delay(200); 
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW); 
        break;
        
      case G1_RADIO_OFF:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        xbee.println(radio_off_cmd);
        delay(200);
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW); 
        break;
        
      case G1_FAN_ON:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        xbee.println(fan_on_cmd);
        delay(200);
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW); 
        break;
        
      case G1_FAN_OFF:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        xbee.println(fan_off_cmd);
        delay(200);
        digitalWrite(GREEN, HIGH);
        delay(1000);
        digitalWrite(GREEN, LOW); 
        break;
        
      case G1_SHUTDOWN:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        digitalWrite(WHITE, LOW);
        group = GROUP_0;
        break;  
      }
      break;
    }
}


void flashLEDs(){}
