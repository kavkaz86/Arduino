#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
}


void loop(){
  
  int val_1 = analogRead(A0); delay(10);
  int val_2 = analogRead(A1); delay(10); 
  
  float UV = (5*5000*(val_1 - val_2))/1023 ; 
  
  if(UV>=0){
    lcd.print(UV); lcd.print(" umol m-2 s-1");
    delay(5000); 
    lcd.clear(); 
  }
  
  
  int val_3 = analogRead(A2); delay(10); 
  int val_4 = analogRead(A3); delay(10); 
  
  float quant= (5*5000*(val_3 - val_4))/1023 ; 
  
  if(quant>=0){
    lcd.print(quant); lcd.print(" PPF");
    delay(5000); 
    lcd.clear(); 
  }
  
}
