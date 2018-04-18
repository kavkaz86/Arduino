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
EasyVR easyvr(port);

//Groups and Commands
enum Groups
{
  GROUP_1  = 1,
  GROUP_16 = 16,
};

enum Group1 
{
  G1_WORD = 0,
  G1_EXCEL = 1,
  G1_EMAIL = 2,
  G1_CHROME = 3,
  G1_DOCUMENTS = 4,
  G1_RESTART = 5,
  G1_SHUTDOWN = 6,
};

enum Group16
{
  G16_SYSTEM_ON = 0,
};


EasyVRBridge bridge;

int8_t group, idx;

int serInLen = 25;

char serInString[25];

int pId =0;

char buffer[5]; 

void setup()
{
  
  pinMode(11,OUTPUT); 
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6, INPUT);
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
    //Serial.println("EasyVR not detected!");
    for (;;);
  }
  
  //Serial.println("EasyVR detected!");
  easyvr.setTimeout(15);
  easyvr.setLanguage(EasyVR::ENGLISH);
  easyvr.setLevel(EasyVR::HARDEST);
  group = EasyVR::PASSWORD;
  
  easyvr.playSound(1, EasyVR::VOL_FULL);
  
  
}

void action();

void loop()
{
  
  digitalWrite(11, HIGH); // Listening
  //Serial.print("Say a command in Group ");
  //Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    blink(); 
  }
  while (!easyvr.hasFinished());
  
  digitalWrite(11, LOW); // LED off
  
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    //Serial.print("Command: ");
    //Serial.print(idx);
    easyvr.dumpCommand(group, idx, name, train);
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout())
      delay(1); 
    int16_t err = easyvr.getError();
    if (err >= 0)
      delay(1);     
  }
}

void action()
{
    switch (group)
    {
    case GROUP_1:
      switch (idx)
      {
      case G1_WORD:
          easyvr.playSound(2, EasyVR::VOL_FULL);          
          Serial.println("#S|SPWORD|[]#"); //Start Winamp
          readSerialString(serInString, 1000);
          pId= atoi(serInString);        
        break;
      case G1_EXCEL:
          easyvr.playSound(2, EasyVR::VOL_FULL);
          Serial.println("#S|SPEXCEL|[]#"); //Start Winamp
          readSerialString(serInString, 1000);
          pId= atoi(serInString);    
        break;
      case G1_EMAIL:
          easyvr.playSound(2, EasyVR::VOL_FULL);
          Serial.println("#S|SPOUTLOOK|[]#"); //Start Winamp
          readSerialString(serInString, 1000);
          pId= atoi(serInString);
        break;
      case G1_CHROME:
          easyvr.playSound(2, EasyVR::VOL_FULL);
          Serial.println("#S|SPCHROME|[]#"); //Start Winamp
          readSerialString(serInString, 1000);
          pId= atoi(serInString);
        break;
      case G1_DOCUMENTS:
          easyvr.playSound(2, EasyVR::VOL_FULL);
          Serial.println("#S|SPDOCO|[]#"); //Start Winamp
          readSerialString(serInString, 1000);
          pId= atoi(serInString);
        break;
      case G1_RESTART:
          easyvr.playSound(2, EasyVR::VOL_FULL);
        break;
      case G1_SHUTDOWN:
          easyvr.playSound(3, EasyVR::VOL_FULL);
        break;
      }
      break;
    case GROUP_16:
      switch (idx)
      {
      case G16_SYSTEM_ON:
        easyvr.playSound(5, EasyVR::VOL_FULL);
        group = 1; 
        break;
      }
      break;
    }
}

void blink(){
    
    while(group==1){
      switch(digitalRead(6)){
        
        case 0: 
          for(int i=10; i>6; i--){
            digitalWrite(i, HIGH);
            delay(100);
          }
          
          for(int i=10; i>6; i--){
            digitalWrite(i, LOW);
            delay(50);
          }
          /*
          digitalWrite(10, HIGH);
          delay(100);
          digitalWrite(9, HIGH);
          delay(100);
          digitalWrite(8, HIGH);
          delay(100);
          digitalWrite(7, HIGH);
          delay(100);
          digitalWrite(10, LOW);
          delay(50);
          digitalWrite(9, LOW);
          delay(50);
          digitalWrite(8, LOW);
          delay(50);
          digitalWrite(7, LOW);
          delay(50); */
          
        break;
        
        case 1:
          
          for(int i=7; i<11; i++){
            digitalWrite(i, HIGH);
            delay(100);
          }
          
          for(int i=7; i<11; i++){
            digitalWrite(i, LOW);
            delay(50);
          }
          /*
          digitalWrite(7, HIGH);
          delay(100);
          digitalWrite(8, HIGH);
          delay(100);
          digitalWrite(9, HIGH);
          delay(100);
          digitalWrite(10, HIGH);
          delay(100);
          digitalWrite(7, LOW);
          delay(50);
          digitalWrite(8, LOW);
          delay(50);
          digitalWrite(9, LOW);
          delay(50);
          digitalWrite(10, LOW);
          delay(50); */
          
        break;
      }
      return;
    }    
}

void readSerialString (char *strArray,long timeOut) 

{

   long startTime=millis();

   int i;



   while (!Serial.available()) {

      if (millis()-startTime >= timeOut) {

         return;

      }

   }

   while (Serial.available() && i < serInLen) {

      strArray[i] = Serial.read();

      i++;

   }

}
