#include <LiquidCrystal.h>
#include "SoftwareSerial.h"
#include <Streaming.h>

float Sen1=0;

float Sen2=0;

float Sen3=0; 

SoftwareSerial xtend(7,6);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup(){
    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // initialize the serial communications:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  xtend.begin(9600);
 
  lcd.print("Initializing"); 
  Welcome(); 
  delay(1000); 
  lcd.clear();  
}

void loop()
{ 
  Sen1=analogRead(A0) * (5.0 / 1023.0);
  xtend << "A" << Sen1 << endl; 
  delay(100);   
  
  lcd.print("Flame-1= "); lcd.print(Sen1); lcd.print(" V"); delay(1000); 
  
  if(Sen1>2.5){
    
    lcd.setCursor(0, 1);
    lcd.print("SAFE CONDITIONS"); 
  }
  else if(Sen1 < 2.5){
    lcd.setCursor(0,1);
    lcd.print("DANGER"); 
  }
  
  delay(2000); 
      
  
  lcd.clear();
  
  Sen2=analogRead(A1) * (5.0 / 1023.0);
  xtend << "B" << Sen2 << endl; 
  
  delay(100);   
  
  lcd.print("Flame-2= "); lcd.print(Sen2); lcd.print(" V"); delay(1000); 
  
  if(Sen2>2.5){
    
    lcd.setCursor(0, 1);
    lcd.print("SAFE CONDITIONS"); 
  }
  else if(Sen2 < 2.5){
    
    lcd.setCursor(0,1);
    lcd.print("DANGER");
     
  }
  
  delay(2000); 
  
  lcd.clear();  
  
  Sen3=analogRead(A2) * (5.0 / 1023.0);
  xtend << "C" << Sen3 << endl; 
  delay(100);   
  
  lcd.print("Flame-3= "); lcd.print(Sen3); lcd.print(" V"); delay(1000); 
  
  if(Sen3>2.5){
    
    lcd.setCursor(0, 1);
    lcd.print("SAFE CONDITIONS"); 
  }
  else if(Sen3 < 2.5){
    lcd.setCursor(0,1);
    lcd.print("DANGER"); 
  }
  
  delay(2000); 
  
  lcd.clear();
   
}

void Welcome(){
  
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight(); 
    // wait a bit:
    delay(150);
  }
  
    // scroll 16 positions (display length + string length) to the left
    // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }
  
  // delay at the end of the full loop:
  delay(1000);
}
