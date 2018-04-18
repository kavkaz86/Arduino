void setup(){
  
  analogReference(INTERNAL); 
  Serial.begin(9600); 
  
}

void loop(){
  
  int val_1 = analogRead(A0);
  int val_2 = analogRead(A1);
  delay(10); 
  int val_3 = analogRead(A2); 
  int val_4 = analogRead(A3); 
  delay(10); 
  
  Serial.print("Val_1= "); Serial.println(val_1); 
  Serial.print("Val_2= "); Serial.println(val_2);
  Serial.print("Val_3= "); Serial.println(val_3);
  Serial.print("Val_4= "); Serial.println(val_4);
  delay(500); 
  Serial.print("Val_1 - Val_2= "); Serial.println(val_1 - val_2);
  Serial.print("Val_3 - Val_4= "); Serial.println(val_3 - val_4); 
  
  delay(2000); 
}

