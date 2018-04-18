/*                                                        * /
 *                                                        * / 
 *      EARTHQUAKE SIMULATOR FOR ECU SCIENCE ACADEMY      * /
 *                                                        * /
 *      BY AMRO QANDOUR                                   * /
 */


/*        LIBRARY DEFINITIONS        */ 
#include <LiquidCrystal.h>

/*        LCD Library Pin Selection  */
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

/*        POTENTIOMETER ANALOG INPUTS  */
const int POT_1 = A0; 
const int POT_2 = A1;  
const int POT_3 = A2;  
const int POT_4 = A3;  

/*        VIBRATION SENSORS INPUTS    */
const int VIB_1 = 13;
const int VIB_2 = 11;
const int VIB_3 = 10;
const int VIB_4 = 9;

/*        GLOBAL VARIABLES            */
int POT1_IN = 0;        
int POT2_IN = 0;
int POT3_IN = 0;
int POT4_IN = 0;

int POT1_OUT = 0;        
int POT2_OUT = 0;
int POT3_OUT = 0;
int POT4_OUT = 0;

/*        INITIALISATION FUNCTION    */
void setup() {
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  analogReference(DEFAULT);
  // ROW 0: 
  setROW0();
  
  // ROW 1:
  setROW1();
}

/*        ROW-1 LCD SCREEN SETUP FUNCTION      */
void setROW1(){
  lcd.setCursor(0, 1); 
  lcd.setCursor(3,1);
  lcd.print("|");
  lcd.setCursor(7,1); 
  lcd.print("|");
  lcd.setCursor(11,1);
  lcd.print("|");
  lcd.setCursor(15,1);
  lcd.print("|"); 
}

/*      ROW-0 LCD SCREEN SETUP FUNCTION      */
void setROW0(){
  lcd.print("PO1");
  lcd.setCursor(3,0); 
  lcd.print("|");
  lcd.setCursor(4,0);
  lcd.print("PO2");
  lcd.setCursor(7,0); 
  lcd.print("|");
  lcd.setCursor(8,0);
  lcd.print("PO3");
  lcd.setCursor(11,0);
  lcd.print("|"); 
  lcd.setCursor(12,0);
  lcd.print("PO4");
  lcd.setCursor(15,0);
  lcd.print("|");
}

/*      MAIN PROGRAM FUNCTION:                   * /
 *      - READ POTENTIOMETER ANALOG READING      * /
 *      - MAPS THE READING BETWEEN 0-255         * /
 *      - OUTPUTS PWM PULSE ACCORDING TO VALUE   * / 
 *      - DISPLAYS THE MAPPED VALUE ON LCD       * / 
*/

void loop() {
    
  POT1_IN = analogRead(POT_1); delay(2);
  POT1_OUT = map(POT1_IN, 0, 1023, 0, 255);
  analogWrite(VIB_1, POT1_OUT); delay(100);
  
  POT2_IN = analogRead(POT_2); delay(2);
  POT2_OUT = map(POT2_IN, 0, 1023, 0, 255);
  analogWrite(VIB_2, POT2_OUT); delay(100);
  
  POT3_IN = analogRead(POT_3); delay(2);
  POT3_OUT = map(POT3_IN, 0, 1023, 0, 255);
  analogWrite(VIB_3, POT3_OUT); delay(100);
  
  POT4_IN = analogRead(POT_4); delay(2); 
  POT4_OUT = map(POT4_IN, 0, 1023, 0, 255);
  analogWrite(VIB_4, POT4_OUT); delay(100);  
  
  /*      DISPLAY ON LCD          */
  lcd.setCursor(0, 1);                            
  lcd.print(POT1_OUT);
  
  lcd.setCursor(4,1);
  lcd.print(POT2_OUT);
 
  lcd.setCursor(8,1);
  lcd.print(POT3_OUT);
  
  lcd.setCursor(12,1);
  lcd.print(POT4_OUT); 
  
  delay(400);
  
  /*      CLEAR VALS FROM ROW-1      */
  for(int i=0; i < 3; i++){
    lcd.setCursor(i, 1);
    lcd.write(' '); 
  }
  for(int i=4; i < 7; i++){
    lcd.setCursor(i, 1);
    lcd.write(' '); 
  }
  for(int i=8; i < 11; i++){
    lcd.setCursor(i, 1);
    lcd.write(' '); 
  }
  for(int i=12; i < 15; i++){
    lcd.setCursor(i, 1);
    lcd.write(' '); 
  } 
  
}


  
  
  
