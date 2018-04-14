#include <MsTimer2.h>           //Timer interrupt function library
#include  "Streaming.h"
#include  "SoftwareSerial.h"

/*
   RF Module
*/
#define M0  5
#define M1  6
#define TXD  7
#define RXD  8
#define AUXD  9
uint8_t chan = 34;
uint8_t rt26AddrH = 0;
uint8_t rt26AddrL = 26;


int pbIn = 0;                    // Define interrupt 0 that is digital pin 2
int ledOut = 13;                 // Define the indicator LED pin digital pin 13
int number = 0;                  //Interrupt times
volatile int state = LOW;         // Defines the indicator LED state, the default is not bright

SoftwareSerial  rf(RXD, TXD);

void setup()
{

  rf.begin(9600);
  pinMode(ledOut, OUTPUT);//
  pinMode(AUXD, INPUT_PULLUP);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  delay(25);
  digitalWrite(M1, LOW);
  delay(25);
  Serial.begin(115200);
  while (!digitalRead(AUXD)) {
    Serial << digitalRead(AUXD) << endl;
    delay(500);
  }
  attachInterrupt(pbIn, stateChange, FALLING); // Set the interrupt function, interrupt pin is digital pin D2, interrupt service function is stateChange (), when the D2 power change from high to low , the trigger interrupt.
  MsTimer2::set(1000, Handle); // Set the timer interrupt function, running once Handle() function per 1000ms
  MsTimer2::start();//Start timer interrupt function

}

void loop()
{
  if (state == HIGH) //When a moving object is detected, the ledout is automatically closed after the light 2S, the next trigger can be carried out, and No need to reset. Convenient debugging.
  {
    //rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Code: MW|" << "State: " << state << "|Trigger: " << number << endl; delay(250);
    //number = 0; //Cleare the number, so that it does not affect the next trigger
    delay(2000);
    state = LOW;
    number = 0;
    digitalWrite(ledOut, state);    //turn off led
  }

}


void stateChange()  //Interrupt service function
{
  number++;  //Interrupted once, the number +1

}

void Handle()   //Timer service function
{
  if (number > 5) //If in the set of the interrupt time the number more than 1 times, then means have detect moving objects,This value can be adjusted according to the actual situation, which is equivalent to adjust the threshold of detection speed of moving objects.
  {
    rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Code: MW|" << "State: " << state << "|Trigger: " << number << endl; delay(250);
    number = 0; //Cleare the number, so that it does not affect the next trigger
    state = HIGH;
    digitalWrite(ledOut, state);    //light led
  }
  else
    number = 0; //If in the setting of the interrupt time, the number of the interrupt is not reached the threshold value, it is not detected the moving objects, Cleare the number.
}
