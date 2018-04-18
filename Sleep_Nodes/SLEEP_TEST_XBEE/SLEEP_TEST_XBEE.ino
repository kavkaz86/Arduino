#include <SoftwareSerial.h>
#define  pin_sleep  6

SoftwareSerial xbee(2,3);

void setup(){
  xbee.begin(9600);
  pinMode(pin_sleep, OUTPUT); 
  digitalWrite(pin_sleep, HIGH);
  
  
  delay(10000);
 
 
}

void loop(){
  digitalWrite(pin_sleep, LOW);
  delay(15000);
  for(int i =0 ; i< 5; i++){
    
    xbee.print(i); delay(100); 
    xbee.println(" I'm awake"); delay(1000);
  }
   
  digitalWrite(pin_sleep, HIGH);
  delay(15000); 
  for(int i = 0; i < 5; i++){
    xbee.print(i); 
    xbee.println(" I'm sleeping"); 
    delay(1000);
  }
 
  delay(500); 
} 
  
  
