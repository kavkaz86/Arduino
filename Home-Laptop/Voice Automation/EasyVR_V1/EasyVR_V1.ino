#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #include "SoftwareSerial.h"
  #include "Streaming.h"
  SoftwareSerial port(12,13);
#else // Arduino 0022 - use modified NewSoftSerial
  #include "WProgram.h"
  #include "NewSoftSerial.h"
  NewSoftSerial port(12,13);
#endif

#include "EasyVR.h"

EasyVR easyvr(port);

int8_t set = 0;
int8_t group = 0;
uint32_t mask = 0;  
uint8_t train = 0;
char name[32];
bool useCommands = true;

EasyVRBridge bridge;

void setup()
{
  // bridge mode?
  if (bridge.check())
  {
    cli();
    bridge.loop(0, 1, 12, 13);
  }
  // run normally
  Serial.begin(9600);
  port.begin(9600);

  if (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    for (;;);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(5);
  easyvr.setLanguage(EasyVR::ENGLISH);
  
  int16_t count = 0;
  
  Serial.print("Sound table: ");
  if (easyvr.dumpSoundTable(name, count))
  {
    Serial.println(name);
    Serial.print("Sound entries: ");
    Serial.println(count);
  }
  else
    Serial.println("n/a");
  
  group = 0;
  mask |= 1; // force to use trigger
  useCommands = (mask != 1);
}

const char* ws0[] =
{
  "ROBOT",
};
const char* ws1[] =
{
  "ACTION",
  "MOVE",
  "TURN",
  "RUN",
  "LOOK",
  "ATTACK",
  "STOP",
  "HELLO",
};
const char* ws2[] =
{
  "LEFT",
  "RIGHT",
  "UP",
  "DOWN",
  "FORWARD",
  "BACKWARD",
};
const char* ws3[] =
{
  "ZERO",
  "ONE",
  "TWO",
  "THREE",
  "FOUR",
  "FIVE",
  "SIX",
  "SEVEN",
  "EIGHT",
  "NINE",
  "TEN",
};
const char** ws[] = { ws0, ws1, ws2, ws3 };

void loop()
{
  
  
  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)
  
  Serial << "Say (ZERO) to switch ON kettle or say (ONE) to switch it OFF" << endl; 
  set=3; 
  easyvr.recognizeWord(set);
  
  while (!easyvr.hasFinished());
  
  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  int16_t idx = easyvr.getWord();
  
  switch(idx){
    
    case 0: Serial << "You said " << easyvr.getWord() << " = " << ws[set][idx] << endl;  // TURN ON DEVICE 1
            pinMode(8, OUTPUT); 
            digitalWrite(8, HIGH);  
            break;
            
    case 1: Serial << "You said " << easyvr.getWord() << " = " << ws[set][idx] << endl;  // TURN OFF DEVICE 1
            pinMode(8, OUTPUT); 
            digitalWrite(8, LOW);  
            break;
            
    case 2: Serial << "You said " << easyvr.getWord() << " = " << ws[set][idx] << endl;  // TURN ON DEVICE 2
            pinMode(7, OUTPUT); 
            digitalWrite(7, HIGH);  
            break;
            
    case 3: Serial << "You said " << easyvr.getWord() << " = " << ws[set][idx] << endl; // TURN OFF DEVICE 2
            pinMode(7, OUTPUT); 
            digitalWrite(7, LOW);  
            break;
            
    default: if(easyvr.isTimeout()){    //FOR ERRORS 
              Serial << "Time Out" << endl; }
             else 
               Serial << "ERROR = " << _HEX(easyvr.getError()) << endl; 
             break;       
  }
 
} 
  /* 
  if (idx >= 0)
  {
    Serial.print("Word: ");
    Serial.print(easyvr.getWord());
    Serial.print(" = ");
    if (useCommands)
      Serial.println(ws[group][idx]); 
    else
      Serial.println(ws[set][idx]);
    // ok, let's try another set  
    easyvr.playSound(0, EasyVR::VOL_FULL);
  }
    else // errors or timeout
    {
      if (easyvr.isTimeout())
        Serial.println("Timed out, try again...");
      int16_t err = easyvr.getError();
      if (err >= 0)
      {
        Serial.print("Error ");
        Serial.println(err, HEX);
      }
    }
  }
*/

