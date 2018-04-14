#include  "SoftwareSerial.h" 
#include <PrintEx.h>

using namespace ios;
/*
 * PIR sensor tester
 */
#define NODE_ID "NODE ADDR = 1"
#define NODE_LOCATION "NODE LOCATION = JO_1002"
#define SENSOR_DETECT  "SENSOR_STATUS = MOTION DETECTED"
#define SENSOR_ACTIVE  "SENSOR_STATUS = READY"

int ledPin = 13;                // choose the pin for the LED
int inputPin = A1;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
SoftwareSerial  xbee(10,9); 

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  xbee.begin(9600); 
  Serial.begin(9600);
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      xbee << NODE_ID << " | " <<  NODE_LOCATION << " | " << SENSOR_DETECT << endl;
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      //xbee.println("Motion detected!");
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      xbee << NODE_ID << " | " << NODE_LOCATION << " | " <<  SENSOR_ACTIVE << endl;
      //xbee.println("Motion ended!");
      pirState = LOW;
    }
  }
}
