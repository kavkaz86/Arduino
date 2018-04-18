#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(6,7); 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define S1 "Sens_1:"
#define S2 "Sens_2:"
#define S3 "Sens_3:"
#define TX "Sending..."
#define SNS "Sensing..."

void setup() {
  
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT); 
  xbee.begin(9600); 
  
  
  lcd.begin(16, 2);
  
}

void loop() {
  
  
  lcd.print(SNS); 
  blink(); 
  
  int Sens_1 = analogRead(A0); 
  
  delay(200); 
  
  int Sens_2 = analogRead(A1); 
  
  delay(200); 
  
  int Sens_3 = analogRead(A2); 
  
  lcd.clear(); 
  lcd.print(S1); 
  lcd.setCursor(0, 1);
  lcd.print(Sens_1); 
  delay(1000); 
  lcd.clear();
  
  lcd.print(S2); 
  lcd.setCursor(0, 1);
  lcd.print(Sens_2); 
  delay(1000); 
  lcd.clear(); 
  
  lcd.print(S3); 
  lcd.setCursor(0, 1);
  lcd.print(Sens_3); 
  delay(1000); 
  lcd.clear();
  
  lcd.print(TX);   
  digitalWrite(9, HIGH); 
  xbee.print("Sens_1: "); xbee.println(Sens_1);
  xbee.print("Sens_2: "); xbee.println(Sens_2);
  xbee.print("Sens_3: "); xbee.println(Sens_3); 
  delay(1000);
  digitalWrite(9, LOW); 
  lcd.clear(); 
  
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

    
    
  
  

