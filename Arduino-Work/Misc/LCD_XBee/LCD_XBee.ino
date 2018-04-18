/*
  LiquidCrystal Library - display() and noDisplay()
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD and uses the 
 display() and noDisplay() functions to turn on and off
 the display.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/LiquidCrystalDisplay

 */

// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(6,7); 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT); 
  xbee.begin(9600); 
  
 // digitalWrite(8, HIGH); 
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
}

void loop() {
  
  
  lcd.print("Sensing..."); 
  blink(); 
  int Sens_1 = analogRead(A0); 
  
  delay(200); 
  
  int Sens_2 = analogRead(A1); 
  
  delay(200); 
  
  int Sens_3 = analogRead(A2); 
  lcd.clear(); 
  
  lcd.print("Sens_1:"); 
  lcd.setCursor(0, 1);
  lcd.print(Sens_1); 
  delay(1000); 
  lcd.clear();
  lcd.print("Sens_2:"); 
  lcd.setCursor(0, 1);
  lcd.print(Sens_2); 
  delay(1000); 
  lcd.clear(); 
  lcd.print("Sens_3:"); 
  lcd.setCursor(0, 1);
  lcd.print(Sens_3); 
  delay(1000); 
  lcd.clear(); 
  lcd.print("Sending...");   
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
    
    digitalWrite(8, HIGH);
    delay(50);
    digitalWrite(8, LOW);
    delay(50);
  }
}
    
    
  
  

