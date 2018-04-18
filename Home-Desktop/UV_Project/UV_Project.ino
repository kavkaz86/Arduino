#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup(){
  
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(12, OUTPUT); 
  
}

void loop(){
  
  /* Apoogee UV Sensor */ 
  int val_1 = analogRead(A0); 
  int val_2 = analogRead(A1); 
 
  int UV = val_1 - val_2 ;
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

  float voltage = 5 * UV * 5000 / 1023.0;
  
  lcd.print("UV= "); lcd.print(voltage); lcd.print(" V"); 
  if(voltage>2.5){
    
    digitalWrite(12, HIGH); delay(5000); 
    digitalWrite(12,LOW);
  }
  
  delay(2000);
  lcd.clear(); 
  /* Apoogee Quantum Sensor */
  
  int val_3 = analogRead(A2); 
  int val_4 = analogRead(A3); 
  
  int quant= val_3 - val_4;
  
  if(quant>2.5){
    
    digitalWrite(12, HIGH); delay(5000); 
    digitalWrite(12,LOW);
  }
  
  float outvoltage= (quant * 5000 * 5)/1023; 
  lcd.print("QT= "); lcd.print(outvoltage); lcd.print(" V"); 

  delay(2000);
  lcd.clear(); 
 
 
   
  
}
