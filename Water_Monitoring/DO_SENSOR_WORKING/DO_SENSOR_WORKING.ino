#include <SoftwareSerial.h>
SoftwareSerial doport(2,3);

String sensorStringdo ="";
boolean do_string = false;
char buffer[10];
char dobuffer[4]; 

float dosensor=0;

float v_out=0;

void setup(){
  
  Serial.begin(9600);
  
  doport.begin(38400);
  
}

void loop(){
  
  delay(1000); 
  
  doport.print("r");
  doport.print("\r");
  
  doport.listen();
  delay(500);
  
  while(doport.available()){
    
    sensorStringdo += (char)doport.read(); 
  }
  
  sensorStringdo.toCharArray(dobuffer, 6); 
  String output = sensorStringdo ; 
  
  Serial.println(output);
  sensorStringdo = "";
}
    
