#include <Wire.h>
#include <LiquidCrystal.h>

float inADC = 0 ;

float inVoltage = 0; 

float RH = 0;

float noiseValue= 0; 

int tmp102Address = 0x48;


LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 


void setup(){
  lcd.begin(16, 2);
  Wire.begin();
}

void loop(){

  float celsius = getTemperature();
  lcd.print("Temp (C)= "); lcd.print(celsius); delay(5000); 
  lcd.clear();
  
  inADC = analogRead(A0);   
  inVoltage = inADC*5/1023;
    
  RH = (inVoltage - 0.958)/0.0307; //RH% 
  lcd.print("Humidity= "); lcd.print(RH); lcd.print("%");  
  delay(5000); 
  lcd.clear(); 
  
  noiseValue = (5*analogRead(A1))/1023;
  
  lcd.print("Noise (V)= "); lcd.print(noiseValue);  
  delay(5000); 
  lcd.clear(); 
}

float getTemperature(){
  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;
  return celsius;
}
