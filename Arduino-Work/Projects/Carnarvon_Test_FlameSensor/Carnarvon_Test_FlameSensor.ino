void setup(){
  
  pinMode(8, INPUT); 
  
  Serial.begin(9600); 
  
}

void loop(){
  float sensor = 5*analogRead(A0)/1023;
  Serial.print("Analog= "); Serial.println(sensor);
  Serial.print("Digital= "); Serial.println(digitalRead(8)); 
  delay(2000); 
 
}

