#include <LiquidCrystal.h>
#include <Streaming.h> 

#define DEBOUNCE 10 

LiquidCrystal lcd(7,6,5,4,3,2);

long previous=0; 
byte buttons[] = {12,11,10,9}; // the analog 0-5 pins are also known as 14-19
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];


int Sens_1= 1023; 
int Sens_2 = 1023; 
int Sens_3 = 1023; 
int Sens_4 = 1023;  
void setup(){
  
  Serial.begin(9600); 
  
  byte i;
  
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  
  //pinMode(13, OUTPUT);
  //digitalWrite(13, LOW); 
  pinMode(18, OUTPUT); 
  pinMode(8, OUTPUT); 
  pinMode(19, OUTPUT); 
  
  lcd.begin(16, 2);
  
  lcd.print("Initializing..."); 
  previous=millis(); 
  
  while(millis() - previous < 5000){
    
    lcd.noDisplay(); 
    delay(500);
    lcd.display(); 
    delay(500);
  }
  lcd.clear(); 
  
  lcd.print("Press any red");
  lcd.setCursor(0,1);
  lcd.print("switch");
  
  
  
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime) {
     // we wrapped around, lets just try again
     lasttime = millis();
  }

  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return;
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();

  for (index = 0; index < NUMBUTTONS; index++) {
    justpressed[index] = 0;       // when we start, we clear out the "just" indicators
    justreleased[index] = 0;

    currentstate[index] = digitalRead(buttons[index]);   // read the button

    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //USB.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}


void loop(){
  
  
  check_switches();      // when we check the switches we'll get the current state

  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
     
      switch(i){
       case 3:
          lcd.clear(); 
          digitalWrite(18, HIGH);
          lcd.print("Sens_1:");
          lcd.setCursor(0,1);
          Sens_1 = analogRead(A0);
          //if(Sens_1 < 800)
            //Serial << "Sens_1" << Sens_1 << endl; 
          lcd.print(Sens_1);
          
          
          break; 
          
       case 2:
          digitalWrite(8, HIGH);
          lcd.clear(); 
          lcd.print("Sens_2:");
          lcd.setCursor(0,1);
          Sens_2 = analogRead(A1);
          //if(Sens_2 < 800)
            //Serial << "Sens_2" << Sens_2 << endl;
          lcd.print(Sens_2);
          
          
          break; 
       
       case 1:
          digitalWrite(19, HIGH); 
          lcd.clear(); 
          lcd.print("Sens_3:");
          lcd.setCursor(0,1);
          Sens_3 = analogRead(A2);
          //if(Sens_3 < 800)
            //Serial << "Sens_3" << Sens_3 << endl;
          lcd.print(Sens_3);
           
          break; 
     
       case 0:
          //digitalWrite(13, HIGH); 
          lcd.clear(); 
          lcd.print("Sens_4:");
          lcd.setCursor(0,1);
          Sens_4 = analogRead(A3);
          //if(Sens_4 < 800)
            //Serial << "Sens_4" << Sens_4 << endl; 
          lcd.print(Sens_4);
          
          
          break; 
      }
      
    }
    
    if (justreleased[i]) {
      switch(i){
       case 3:
        //   Serial << "Sens_1" << Sens_1 << endl;
          digitalWrite(18, LOW);
           break; 
          
       case 2: 
       digitalWrite(8, LOW); 
        //   Serial << "Sens_2" << Sens_2 << endl; 
           break;
       
       case 1:
       //  Serial << "Sens_3" << Sens_3 << endl;
       digitalWrite(19, LOW);
         break;
     
       case 0:
       //  Serial << "Sens_4" << Sens_4 << endl;
       //digitalWrite(13, LOW);
         break;
      }
      //USB.print(i, DEC);
      //USB.println(" Just released");
      // remember, check_switches() will CLEAR the 'just pressed' flag
    } 
    
    if (pressed[i]) {
     
      switch(i){
        
        case 3:
           //Serial << "Sens_1" << Sens_1 << endl;
           break; 
          
       case 2: 
           //Serial << "Sens_2" << Sens_2 << endl; 
           break;
       
       case 1:
         //Serial << "Sens_3" << Sens_3 << endl;
         break;
     
       case 0:
        // Serial << "Sens_4" << Sens_4 << endl;
         break;
      }
    }
  }
  
  if(Sens_1 < 800){
            Serial << "Sens_1= " << Sens_1 << endl;
            Sens_1 = 1023;
            delay(1000); 
  }
  else if(Sens_2 < 800){
            Serial << "Sens_2= " << Sens_2 << endl;
            Sens_2 = 1023;
            delay(1000); 
  }         
  else if(Sens_3 < 800){
            Serial << "Sens_3= " << Sens_3 << endl;
            Sens_3 = 1023;
            delay(1000); 
  }
  else if(Sens_4 < 800){
            Serial << "Sens_4= " << Sens_4 << endl;
            Sens_4 = 1023;
            delay(1000); 
  }
  
    //Sens_1 = 0;
    //Sens_2 = 0; 
    //Sens_3 = 0; 
    //Sens_4 = 0;  
            
}
  
  
  
  
  
