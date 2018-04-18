#define DELAY 60000
#define Tx_msg "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRST"
void setup(){
  
  Serial.begin(9600);
  
}

void loop(){
  
  Serial.println(Tx_msg);
  
  delay(DELAY); 
}

  
  
  
  
