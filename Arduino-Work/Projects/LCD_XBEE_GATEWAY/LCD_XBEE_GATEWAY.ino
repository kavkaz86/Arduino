#include "SoftwareSerial.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);

//SoftwareSerial xbee(4,5); 

String inputString=""; 

void setup(){
   
  
   pinMode(8, OUTPUT);
   pinMode(9, OUTPUT);
   Serial.begin(9600); 
  
   lcd.begin(16, 2);
   
  // xbee.begin(9600); 
   
   inputString.reserve(20);
   
   //pinMode(2,OUTPUT); 
   //pinMode(3,OUTPUT);
   
}

void loop(){
  
  if(Serial.available()){
    
    delay(200); 
    
    lcd.clear(); 
    
    inputString = ""; 
    
    while(Serial.available() > 0) {
      
      inputString += (char)Serial.read(); 
      
       
      //lcd.print((char)xbee.read()); 
    } 
    
    lcd.print(inputString);
    
  }
  
  if(inputString == "1") digitalWrite(8, HIGH); 
  if(inputString == "2") digitalWrite(8,LOW);
  if(inputString == "3") digitalWrite(9,HIGH);
  if(inputString == "4") digitalWrite(9,LOW);
  
  
  
}
