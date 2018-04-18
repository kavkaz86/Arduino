#include "Arduino.h"
#include "SoftwareSerial.h"
#include "EasyVR.h"

EasyVR easyvr(Serial);

int8_t group = 0;
uint8_t train = 0;
char name[32];


void setup()
{
  Serial.begin(9600);

  if (!easyvr.detect())
  {
    for (;;);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  easyvr.setTimeout(5);
  easyvr.setLanguage(EasyVR::ENGLISH);
}

void loop()
{
  
  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  easyvr.recognizeCommand(group); 

  do
  {
    
  }
  while (!easyvr.hasFinished());
  
  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off
  
  int16_t idx = easyvr.getCommand();
  
  if (idx >= 0)
  {
    if (easyvr.dumpCommand(group, idx, name, train))
      {
        
        /* 
            
            Action code goes here
        
        */
        
      }
      
    easyvr.playSound(0, EasyVR::VOL_FULL);
  }
  
  else // errors or timeout
    {
      
      if (easyvr.isTimeout())
      {  
        /* 
           
           Timeout Code
           Goes Here 
           
        */ 
      }
      
      if (easyvr.getError() >= 0)
      {
        /* 
        
            Error code goes here
            
        */
        
      }
    }
}


