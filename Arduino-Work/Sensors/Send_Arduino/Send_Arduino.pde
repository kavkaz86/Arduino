 

void setup(){
  
  //Serial.begin(9600,0); 
  
 XBee.begin(0,9600);
 XBee.setMode(XBEE_ON);
    
   
    
}

void loop(){
  
  
  XBee.print("1"); 
  
  delay(2000); 
  
}
