#include <LiquidCrystal.h>
#include <SoftwareSerial.h> 
#include <Streaming.h>


byte micro[8] = {
  B00000,
  B00000,
  B00000,
  B10010,
  B10010,
  B10010,
  B11100,
  B10000
};

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

SoftwareSerial xbee(8,9);


void setup(){
  
  lcd.createChar(0, micro);
  Serial.begin(9600); 
  xbee.begin(9600); 
  analogReference(INTERNAL);
  lcd.begin(16, 2);
  lcd.write(uint8_t(0)); lcd.write("mol.s^-1.m^-2:"); 
  
 
}

void loop(){
  
  lcd.setCursor(0,1); 
  
  lcd.print(SQ110(A0));
 
  xbee.println(SQ110(A0)); 
  
  delay(1000); 

  
}
  


long SQ110(int pin){
  
  int rawSU; 
  float milliVolts; 
  float UV; 
  
  rawSU = analogRead(pin); 
  
  milliVolts = ((float(rawSU) * 1100)/1023); 
  
  UV = milliVolts * 5; 
  
  return (float(UV)); 
}
  
  
  
  
  
