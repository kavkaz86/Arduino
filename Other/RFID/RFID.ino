#include "SoftwareSerial.h"

int RFIDResetPin = 13;

SoftwareSerial RFID(6,7);

void setup(){
  Serial.begin(9600);
  RFID.begin(9600); 
  pinMode(RFIDResetPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);

}

void loop(){

  char tagString[13];
  int index = 0;
  boolean reading = false;
  
    
    
    while(!RFID.available()); 
    while(RFID.available()){  
    int readByte = RFID.read(); //read next available byte

    if(readByte == 2) reading = true; //begining of tag
    if(readByte == 3) { 
      reading = false; //end of tag
      Serial.println(""); 
      delay(200);  
    }

    if(reading && readByte != 2 && readByte != 10 && readByte != 13){
      //store the tag
      tagString[index] = readByte;
      Serial.print(tagString[index]); 
      index ++;
    }
  }
    clearTag(tagString); //Clear the char of all value
    resetReader(); //Reset the RFID reader
    
}

void resetReader(){
///////////////////////////////////
//Reset the RFID reader to read again.
///////////////////////////////////
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(150);
}

void clearTag(char one[]){
///////////////////////////////////
//clear the char array by filling with null - ASCII 0
//Will think same tag has been read otherwise
///////////////////////////////////
  for(int i = 0; i < strlen(one); i++){
    one[i] = 0;
  }
}
