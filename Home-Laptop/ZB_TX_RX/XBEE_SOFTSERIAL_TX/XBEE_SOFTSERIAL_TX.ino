#include <SoftwareSerial.h>
#include <Streaming.h>
SoftwareSerial mySerial(2, 3);

void setup(){
  
  Serial.begin(9600);
  mySerial.begin(9600); 
  
}

void loop(){
  
  mySerial.println("HELLO");
  
  delay(2000);
}

