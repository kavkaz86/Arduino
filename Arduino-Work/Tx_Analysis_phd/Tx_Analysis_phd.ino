#define DELAY 60000


#define Tx_Message "ABCDEFGHIKLMNOPQRSTUVWXYZ123456789ABCDEFGHIKLMNOPQRSTUVWXYZ123456789"

void setup(){
  
  Serial.begin(9600); 
  
}

void loop(){
  
  Serial.println(Tx_Message); 
  
  delay(DELAY); 
  
}

  
  
  
  
  
  
