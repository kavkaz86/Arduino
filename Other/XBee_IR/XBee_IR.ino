
#include "IRTemp.h"
#include  "SoftwareSerial.h"
#include  "Streaming.h"

SoftwareSerial  xbee(2,3); 

static const int  XBEE_SLEEP  = 7;
static const byte PIN_DATA    = 8; // Choose any pins you like for these
static const byte PIN_CLOCK   = 9;
static const byte PIN_ACQUIRE = 10;

static const TempUnit SCALE=CELSIUS;  // Options are CELSIUS, FAHRENHEIT

IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);

void setup(void) { 
  xbee.begin(9600); 
  Serial.begin(9600);
  pinMode(XBEE_SLEEP, OUTPUT);
  
}

void loop(void) {
  digitalWrite(XBEE_SLEEP, LOW);
  delay(5000);
  
  float irTemperature = irTemp.getIRTemperature(SCALE);
  printTemperature("IR", irTemperature);

  float ambientTemperature = irTemp.getAmbientTemperature(SCALE);
  printTemperature("Ambient", ambientTemperature);
  
  digitalWrite(XBEE_SLEEP, HIGH);
  delay(5000);
}

void printTemperature(
  char  *type,
  float  temperature) {
  //xbee << type << " temperature: "; 
  //xbee.print(type);
  //xbee.print(" temperature: ");

  if (isnan(temperature)) {
    //xbee.println("Failed");
    xbee << "Failed" << endl;
  }
  else {
    xbee << type << " temperature: " << temperature << (SCALE == FAHRENHEIT  ?  " F"  :  " C") << endl;
    //xbee.print(temperature);
    //xbee.println(SCALE == FAHRENHEIT  ?  " F"  :  " C");
  }
}
