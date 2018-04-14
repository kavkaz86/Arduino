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

const int buttonPin = A0;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 25;    // the debounce time; increase if the output flickers
long previous = 0;

void setup() {
  Serial.begin(9600);
  lora.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AUX, INPUT);


  // set initial LED state
  digitalWrite(ledPin, ledState);
}


bool  rf = false;
void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
        if (rf) {
          while(!digitalRead(AUX));
          lora << "ON" ;
          Serial << "ON" << endl;
          rf = false;
        }
        else {
          while(!digitalRead(AUX));
          lora << "OFF" ;
          Serial << "OFF" << endl;
          rf = true;
        }
        
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);
  //rf = !rf;
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

