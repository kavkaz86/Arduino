#include "SoftwareSerial.h"

SoftwareSerial xbee(2,3); 

int alarm = 0; 

void setup() {
  //xbee.begin(9600); 
  Serial.begin(9600);
  
}

void loop() {
  

    
    while(Serial.available()){
      
      Serial.print((char)Serial.read()); 
    
  }
}

