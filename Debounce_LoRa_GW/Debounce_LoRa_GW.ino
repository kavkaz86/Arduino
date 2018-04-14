#include  "SoftwareSerial.h"
#include  "Streaming.h"

/*
   Globals variables
*/
#define AUX A1
#define TXD 9
#define RXD 10

//Initialise the Software Serial Class
SoftwareSerial  lora(RXD, TXD);

//Data variables
String  data_IN = ""; // Radio TX Data
const int buttonPin = A0;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

void setup() {

  Serial.begin(9600);
  lora.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AUX, INPUT);


  // set initial LED state
  digitalWrite(ledPin, ledState);

 // lora << "OFF" << endl;
}

bool  rf = false;

int counta = 0;
void loop() {
  
  while(digitalRead(buttonPin)){
    counta++;
    if(counta == 1){
      lora.print("OFF");
      delay(2000);
      lora.print("ON");
    }
  }
  // read the state of the switch into a local variable:
  
  counta=0; 

  
  
}

