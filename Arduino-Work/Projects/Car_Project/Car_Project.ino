#include <LiquidCrystal.h>
#include <Wire.h>
int tmp102Address = 0x48;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,6, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  Wire.begin();

}


void loop(){
  
  float celsius = getTemperature();
   float fahrenheit = (1.8 * celsius) + 32;
  lcd.print("Temp: ");
  lcd.setCursor(0,1); 
  lcd.print(celsius); lcd.print("C / "); lcd.print(fahrenheit); lcd.print("F"); 
  

  delay(5000); //just here to slow down the output. You can remove this
  lcd.clear(); 
  
  float Hum = 5*analogRead(0) / 1023; 
  
  float RH = (Hum - 0.958)/0.0307; //RH% 
  
  lcd.print("Humidity:");
  lcd.setCursor(0,1); 
  lcd.print(Hum); lcd.print("V / "); lcd.print(RH); lcd.print("%"); 
  delay(5000); 
  lcd.clear();
  
  int sound = analogRead(A1); 
  float sound_v = 5*sound/1023; 
  
  lcd.print("Sound:"); 
  lcd.setCursor(0,1); 
  lcd.print(sound); lcd.print(" / "); lcd.print(sound_v); lcd.print(" V"); 
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

