#if defined(ARDUINO) && ARDUINO >= 100

  #include "Arduino.h"

  #include "SoftwareSerial.h"

  //SoftwareSerial port(12,13);

#else // Arduino 0022 - use modified NewSoftSerial

  #include "WProgram.h"

  #include "NewSoftSerial.h"

  NewSoftSerial port(12,13);

#endif


#include "Streaming.h"
#include "EasyVR.h"

EasyVR easyvr(Serial);

SoftwareSerial xbee(2,3); 

SoftwareSerial xtend(4,5); 





//Groups and Commands

enum Groups

{

  GROUP_0  = 0,

  GROUP_1  = 1,

};



enum Group0 

{

  G0_START = 0,

};



enum Group2 

{

  G2_COFFEE_ON = 0,

  G2_COFFEE_OFF = 1,

  G2_LIGHT_ON = 2,

  G2_LIGHT_OFF = 3,
  
  G2_STOP = 4, 
  
 

};





EasyVRBridge bridge;



int8_t group, idx, wake; 



void setup()

{  
  
  xtend.begin(9600); 
  
  xbee.begin(9600); 
  
  Serial.begin(9600);
  
  pinMode(6,OUTPUT); 
  
  digitalWrite(6,HIGH); 



  if (!easyvr.detect())

  {

    for (;;);

  }



  easyvr.setPinOutput(EasyVR::IO1, LOW);

  easyvr.setTimeout(5);

  easyvr.setLanguage(5);



  group = EasyVR::TRIGGER; //<-- start group (customize)
  
  easyvr.playSound(1, EasyVR::VOL_FULL);
  
  

}



void action();

void loop()

{  
  
  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  easyvr.recognizeCommand(group);



  do

  {    

  }

  while (!easyvr.hasFinished());

  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getCommand();

  if (idx >= 0)

  {

    uint8_t train = 0;

    char name[32];
    

    if (easyvr.dumpCommand(group, idx, name, train)) xbee << name ;

    else  easyvr.playSound(0, EasyVR::VOL_FULL);

    action();

  }

  else // errors or timeout

  {

    if (easyvr.isTimeout()) {
    
        easyvr.getError();
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

      case G0_START:
          
         easyvr.playSound(2, EasyVR::VOL_FULL);
         
         group = 2; 

        break;

      }

      break;

    case GROUP_1:

      switch (idx)

      {

      case G1_COFFEE_ON:
  
        easyvr.playSound(5, EasyVR::VOL_FULL);

        break;

      case G1_COFFEE_OFF:
        
        easyvr.playSound(5, EasyVR::VOL_FULL);
        
        break;

      case G1_LIGHT_ON:
    
        easyvr.playSound(5, EasyVR::VOL_FULL);

        break;

      case G1_LIGHT_OFF:
      
        easyvr.playSound(5, EasyVR::VOL_FULL);

        break;    

      }   

      break;

    }

}
