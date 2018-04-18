#include <LiquidCrystal.h>
#include <SoftwareSerial.h> 

#define TX "Sending..."
#define SNS "Sensing..."

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

SoftwareSerial xbee(8,9);


void setup(){
  
  Serial.begin(9600); 
  //xbee.begin(9600); 
  analogReference(INTERNAL);
  lcd.begin(16, 2);
  
 
}

void loop(){
  
  lcd.print(SNS); 
  blink(); 
  lcd.clear(); 
  
  lcd.print("UV: ");
  lcd.setCursor(0,1); 
 
  lcd.print(float(SU100(A0))); lcd.print(" umolm-2s-1"); 
  
  delay(5000);
  lcd.clear(); 
 
  lcd.print(TX);
  blink();  
  
  Serial.print(float(SU100(A0))); 
  
  delay(1000); 
  
  lcd.clear();
  
}
  


long SU100(int pin){
  
  int rawSU; 
  float milliVolts; 
  float UV; 
  
  rawSU = analogRead(pin); 
  
  milliVolts = ((float(rawSU) * 1100)/1023); 
  
  UV = milliVolts * 5; 
  
  return (long(UV)); 
}

void blink(){
  
  long startTime=millis();

  while (millis()-startTime < 3000) {
    
    lcd.noDisplay();
    digitalWrite(8, HIGH);
    delay(50);
    digitalWrite(8, LOW);
    lcd.display();
    delay(50);
  }
}
  
  
  
  
  
