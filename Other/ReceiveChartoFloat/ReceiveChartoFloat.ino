#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // RX, TX

String inputString = "";
float inData = 0; 


void setup(){
  xbee.begin(9600);
  Serial.begin(9600); 
  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  
}

 

void loop(){
  
  if(xbee.available()){
     
    delay(200); 
    
    while (xbee.available()) {
    // get the new byte:
    char inChar = (char)xbee.read(); 
    // add it to the inputString:
    inputString += inChar;
    }
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    inData = StrToFloat(inputString);
    Serial.println(inData); 
    inputString = "";
      
  }
  
  
  if(inData >= 0 && inData <=  ){
    
    digitalWrite(8, HIGH);
     
  }
  else if(inData > 1 && inData <=2){
   
    digitalWrite(9, HIGH);
     
  }
  else if(inData > 2){
    digitalWrite(10, HIGH);
  } 
 
} 


float StrToFloat(String str){
  char carray[str.length() + 1]; //determine size of the array
  str.toCharArray(carray, sizeof(carray)); //put str into an array
  return atof(carray);
}
    
     
    
    
