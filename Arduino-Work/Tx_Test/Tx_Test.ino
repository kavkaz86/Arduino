#define DELAY 1000

#define Tx_msg "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRST"
#define Battery_Min 560

void setup() {
  
  Serial.begin(9600);
  
}

void loop() {  
  
  Serial.println(Tx_msg);
  
  delay(DELAY); 
  
}
