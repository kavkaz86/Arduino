#define  pin_sleep  3


void setup(){
  pinMode(pin_sleep, OUTPUT); 
  
  Serial.begin(9600);
  
  //delay(10000);
  //digitalWrite(pin_sleep, HIGH);
 
}

void loop(){
  digitalWrite(pin_sleep, LOW);
  delay(15000);
  for(int i =0 ; i< 5; i++){
    
    //Serial.print(i); delay(100); 
    Serial.print("A:22.2"); delay(100); Serial.print('\r'); delay(100);
  }
   
  digitalWrite(pin_sleep, HIGH);
  delay(2000); 
  for(int i = 0; i < 5; i++){
    Serial.print(i); 
    Serial.println(" I'm sleeping"); 
    delay(1000);
  }
 
  delay(500); 
} 
  
  
