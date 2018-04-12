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

// Sensor Variables
int pbIn = 0;                    // Define interrupt 0 that is digital pin 2
int pbIn1 = 1;                    // Define interrupt 0 that is digital pin 3
int ledOut = 13;                 // Define the indicator LED pin digital pin 13
int number = 0;                  //Interrupt times
int number1 = 0;                  //Interrupt times
int alarmID = 0;
volatile int state = LOW;         // Defines the indicator LED state, the default is not bright
volatile int state1 = LOW;         // Defines the indicator LED state


SoftwareSerial  rf(RXD, TXD);

void setup()
{

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUXD, INPUT_PULLUP);
  rf.begin(9600);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  Serial.begin(115200);
  while (!digitalRead(AUXD)) {
    Serial << digitalRead(AUXD) << endl;
    delay(25);
  }
  pinMode(ledOut, OUTPUT);//
  attachInterrupt(pbIn, stateChange, FALLING); // Set the interrupt function, interrupt pin is digital pin D2, interrupt service function is stateChange (), when the D2 power change from high to low , the trigger interrupt.
  attachInterrupt(pbIn1, stateChange1, FALLING); // Set the interrupt function, interrupt pin is digital pin D2, interrupt service function is stateChange (), when the D2 power change from high to low , the trigger interrupt.
  MsTimer2::set(1000, Handle); // Set the timer interrupt function, running once Handle() function per 1000ms
  MsTimer2::start();//Start timer interrupt function

}

void loop()
{
  if (state) {

    switch (alarmID) {

      case  1:
        rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Sensor: MW-A, MW-B, State: Active, Interrupt Count (A): " << number << ", Interrupt Count (B): " << number1 << endl;
        delay(2000);
        number = 0;
        number1 = 0;
        break;

      case 2:

        rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Sensor: MW-A, State: Active, Interrupt Count: " << number << endl;
        delay(2000);
        number = 0;
        break;

      case  3:
        rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Sensor: MW-B, State: Active, Interrupt Count: " << number1 << endl;
        delay(2000);

        number1 = 0;
        break;

      default:
        break;
    }

    state = LOW;
    digitalWrite(ledOut, state);    //turn off led
  }
  /*
    if (state == HIGH && state1 == HIGH) //When a moving object is detected, the ledout is automatically closed after the light 2S, the next trigger can be carried out, and No need to reset. Convenient debugging.
    {
    rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Sensor: MW-A, MW-B, State: Active, Interrupt Count (A): " << number << ", Interrupt Count (B): " << number1 << endl;
    delay(2000);
    state = LOW;
    state1 = LOW;
    digitalWrite(ledOut, state);    //turn off led
    number = 0;
    number1 = 0;
    }
    else if (state == HIGH && state1 == LOW) //When a moving object is detected, the ledout is automatically closed after the light 2S, the next trigger can be carried out, and No need to reset. Convenient debugging.
    {
    rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Sensor: MW-A, State: Active, Interrupt Count: " << number << endl;
    delay(2000);
    state = LOW;
    digitalWrite(ledOut, state);    //turn off led
    number = 0;
    }

    else if (state1 == HIGH && state == LOW) //When a moving object is detected, the ledout is automatically closed after the light 2S, the next trigger can be carried out, and No need to reset. Convenient debugging.
    {
    rf << _BYTE(rt26AddrH) << _BYTE(rt26AddrL) << _BYTE(chan) << "Sensor: MW-B, State: Active, Interrupt Count: " << number1 << endl;
    delay(2000);
    state1 = LOW;
    digitalWrite(ledOut, state1);    //turn off led
    number1 = 0;
    } */

}


void stateChange()  //Interrupt service function
{
  number++;  //Interrupted once, the number +1

}

void stateChange1()  //Interrupt service function
{
  number1++;  //Interrupted once, the number +1

}

void Handle()   //Timer service function
{
  if (number > 10 && number1 > 10) //If in the set of the interrupt time the number more than 1 times, then means have detect moving objects,This value can be adjusted according to the actual situation, which is equivalent to adjust the threshold of detection speed of moving objects.
  {
    state = HIGH;
    //digitalWrite(ledOut, state);    //light led
    //number = 0; //Cleare the number, so that it does not affect the next trigger
    alarmID = 1;
  }
  else if (number > 10 && number1 < 10) {
    state = HIGH;
    //digitalWrite(ledOut, state);    //light led
    //number = 0; //Cleare the number, so that it does not affect the next trigger
    alarmID = 2;
  }
  else if (number1 > 10 && number < 10) {
    state = HIGH;
    //digitalWrite(ledOut, state1);    //light led
    //number = 0; //Cleare the number, so that it does not affect the next trigger
    alarmID = 3;
  }
  else {
    number = 0; //If in the setting of the interrupt time, the number of the interrupt is not reached the threshold value, it is not detected the moving objects, Cleare the number.
    number1 = 0;
    alarmID = 0;
  }
}
