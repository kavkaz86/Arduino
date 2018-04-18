#define  BOOZE_BUS  1
#define  MULTINOVA  2
#define  COPS       3
#define  CAR_ACC    4
#define  TRAFFIC    5
#define DEBOUNCE 5  // button debouncer, how many ms to debounce, 5+ ms is usually plenty


const uint8_t  BOOZE_PIN = 4;
const uint8_t  MULTI_PIN = 5;
const uint8_t  COPS_PIN  = 6;
const uint8_t  ACC_PIN   = 7;
const uint8_t  TRAFF_PIN = 8;
byte buttons[] = {BOOZE_PIN, MULTI_PIN, COPS_PIN, ACC_PIN, TRAFF_PIN};
#define NUMBUTTONS sizeof(buttons)

const uint8_t  BOOZE_LED = 9;
const uint8_t  MULTI_LED = 10;
const uint8_t  COPS_LED  = 11;
const uint8_t  ACC_LED   = 12;
const uint8_t  TRAFF_LED = 13;
byte leds[] = {BOOZE_LED, MULTI_LED, COPS_LED, ACC_LED, TRAFF_LED};
#define NUMLEDS sizeof(leds)



volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

void setup(){
  
  byte i;
  
  Serial.begin(9600);
  //Serial.print("Button checker with ");  //   DEBIG  //
 // Serial.print(NUMBUTTONS, DEC);         //   ----   //
 // Serial.println(" buttons");            //   CODE   //

  
  for (i=0; i< NUMLEDS; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  // Run timer2 interrupt every 15 ms 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;
}

SIGNAL(TIMER2_OVF_vect) {
  check_switches();
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
     
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    
    /*     
    Serial.print(index, DEC);
    Serial.print(": cstate=");
    Serial.print(currentstate[index], DEC);
    Serial.print(", pstate=");
    Serial.print(previousstate[index], DEC);
    Serial.print(", press=");
    */
    
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
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}

void loop() {
  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
      
      switch(i){
        
        case 0:
          digitalWrite(BOOZE_LED, HIGH);
          Serial.println(BOOZE_BUS);
          break; 
        case 1:
          digitalWrite(MULTI_LED, HIGH);
          Serial.println(MULTINOVA);
          break; 
        case 2:
          digitalWrite(COPS_LED, HIGH);
          Serial.println(COPS);
          break; 
        case 3:
          digitalWrite(ACC_LED, HIGH);
          Serial.println(CAR_ACC);
          break; 
        case 4:
          digitalWrite(TRAFF_LED, HIGH);
          Serial.println(TRAFFIC);
          break; 
        default:
          break;  
      }
      justpressed[i] = 0;
     // Serial.print(i, DEC);
    //  Serial.println(" Just pressed"); 
      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (justreleased[i]) {
      
      switch(i){
        
        case 0:
          digitalWrite(BOOZE_LED, LOW);
          break; 
        case 1:
          digitalWrite(MULTI_LED, LOW);
          break; 
        case 2:
          digitalWrite(COPS_LED, LOW);
          break; 
        case 3:
          digitalWrite(ACC_LED, LOW);
          break; 
        case 4:
          digitalWrite(TRAFF_LED, LOW);
          break; 
        default:
          break;  
      }
      justreleased[i] = 0;
     // Serial.print(i, DEC);
     // Serial.println(" Just released");
      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    //if (pressed[i]) {
     // Serial.print(i, DEC);
     // Serial.println(" pressed");
      // is the button pressed down at this moment
    //}
  }
  
  if(Serial.available()){
    
    int inData=0;  
    delay(200); 
    
    // get the new byte:
    inData = Serial.read(); 
    // add it to the inputString:
    switch(inData){
        case BOOZE_BUS:
          digitalWrite(BOOZE_LED, HIGH);
          //Serial.println(BOOZE_BUS);
          break; 
        case  MULTINOVA:
          digitalWrite(MULTI_LED, HIGH);
          //Serial.println(MULTINOVA);
          break; 
        case COPS:
          digitalWrite(COPS_LED, HIGH);
          //Serial.println(COPS);
          break; 
        case CAR_ACC:
          digitalWrite(ACC_LED, HIGH);
          //Serial.println(CAR_ACC);
          break; 
        case TRAFFIC:
          digitalWrite(TRAFF_LED, HIGH);
          //Serial.println(TRAFFIC);
          break; 
        default:
          break;  
      }
      delay(1000);
      for (byte i=0; i< NUMLEDS; i++) {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
      }
  }
    
    
}
   
  
  
