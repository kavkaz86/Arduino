#include "SoftwareSerial.h" 
#include "Streaming.h" 

SoftwareSerial xbee(2,3); 

String input=""; 

int serInLen = 25;

char serInString[25];

int pId =0;

int result;

char buffer[5]; 


void setup(){
  
  Serial.begin(9600); 
  
  xbee.begin(9600); 
  
  input.reserve(20); 
  
} 

void loop(){ 
  
  
  if(xbee.available()){
    
    input = "";
    
    while(xbee.available() > 0){
    
    input += (char)xbee.read(); 
    
    }
    
  }
  
  if(input == "EMILIE"){
    
    Serial.println("#S|SPWA|[]#"); //Start Winamp
    readSerialString(serInString, 5000);
    pId= atoi(serInString); 
    
    Serial.print("#S|SENDK|["); //Sending Strokekeys to load playlist
    Serial.print(itoa((pId), buffer, 10));   
    Serial.print("&");
    Serial.print("^"); // Ctrl Key
    Serial.print("+"); // Shift 
    Serial.print("p"); // p  i.e ctrl+shift+p will play songs. 
    Serial.println("]#");

     // wait up to 1000 ms for answer from Gobetwino, answer will be in serInString, answer is 0 if all is OK

    readSerialString(serInString, 1000);
    
  }
  
  if(input == "CLOSE_WINAMP"){
    
    
    
    //Code Here to close Winamp
    
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
    
    
  
