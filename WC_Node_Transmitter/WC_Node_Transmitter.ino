#include  "Streaming.h"
#include  "SoftwareSerial.h"


/*
   Data
*/
#define MAX 200
#define MAX_SQL 200

/*
   LEDs
*/
#define TX_LED 4
#define PWR_LED 10

/*
   RF Module
*/
#define M0  5
#define M1  6
#define TXD  7
#define RXD  8
#define AUXD  9


/*
   NWK Parameters
*/
#define DEBUG true

uint8_t route_table[] = {'0', '0', '0', '1', '0', '2', '0', '3', '0', '4'};
uint8_t bsAddrH = 0;
uint8_t bsAddrL = 0;

uint8_t rt1AddrH = 0;
uint8_t rt1AddrL = 1;

uint8_t rt2AddrH = 0;
uint8_t rt2AddrL = 2;

uint8_t rt3AddrH = 0;
uint8_t rt3AddrL = 3;

uint8_t rt4AddrH = 0;
uint8_t rt4AddrL = 4;

uint8_t rt5AddrH = 0;
uint8_t rt5AddrL = 5;

uint8_t chan = 34;

/*
   Variables
*/
bool stringComplete = false;
long previous = 0;
String inputString = "";         // a String to hold incoming data
unsigned int msgc = 0;
bool  tx_type = false;
String  MESSAGE_1 = "01234567890123456789012345";
String  MESSAGE_2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
bool  ACK_MODE = false;

SoftwareSerial  rf(RXD, TXD);

void setup() {

  inputString.reserve(200);
  pinMode(TX_LED, OUTPUT);
  pinMode(PWR_LED, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUXD, INPUT_PULLUP);

  Serial.begin(115200);
  rf.begin(9600);

  digitalWrite(PWR_LED, HIGH);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  while (!digitalRead(AUXD)) {
    Serial << digitalRead(AUXD) << endl;
    delay(25);
  }

}

void  RF_mode(bool m0, bool m1) {

  digitalWrite(M0, m0);
  digitalWrite(M1, m1);
  delay(25);
  while(!digitalRead(AUX));

}

void loop() {

  while (msgc < 65535) {

    digitalWrite(M0, LOW);
    digitalWrite(M1, LOW);
    while (!digitalRead(AUXD)) {
      Serial << digitalRead(AUXD) << endl;
      delay(25);
    }
    delay(250);
    msgc++;

    if (msgc % 2 == 0) {
      tx_type = 1;
    }
    else  tx_type = 0;

    switch (tx_type) {

      case  0:
        digitalWrite(TX_LED, HIGH);
        rf << _BYTE(rt2AddrH) << _BYTE(rt2AddrL) << _BYTE(chan) << "*MSG_Number = " << msgc << " | Data = " << MESSAGE_1 << '\0'; delay(250);
        while (!digitalRead(AUXD));
        digitalWrite(M0, HIGH);
        digitalWrite(M1, HIGH);
        while (!digitalRead(AUXD));
        delay(100);
        if (DEBUG) {
          Serial << _BYTE(rt2AddrH) << _BYTE(rt2AddrL) << _BYTE(chan) << "*MSG_Number = " << msgc << " | Data = " << MESSAGE_1 << '\0'; delay(250);
        }
        digitalWrite(TX_LED, LOW);
        break;

      case  1:
        digitalWrite(TX_LED, HIGH);
        rf << _BYTE(rt2AddrH) << _BYTE(rt2AddrL) << _BYTE(chan) << "*MSG_Number = " << msgc << " | Data = " << MESSAGE_2 << '\0'; delay(250);
        while (!digitalRead(AUXD));
        digitalWrite(M0, HIGH);
        digitalWrite(M1, HIGH);
        while (!digitalRead(AUXD));
        delay(100);
        if (DEBUG) {
          Serial << _BYTE(rt2AddrH) << _BYTE(rt2AddrL) << _BYTE(chan) << "*MSG_Number = " << msgc << " | Data = " << MESSAGE_2 << '\0'; delay(250);
        }
        digitalWrite(TX_LED, LOW);
        break;

      default:
        digitalWrite(M0, HIGH);
        digitalWrite(M1, HIGH);
        delay(20);
        while (!digitalRead(AUXD));
        break;

    }

    NWK_ACK_FUNCTION(ACK_MODE);

    delay(10000);

  }

  msgc = 0;

}

void  NWK_ACK_FUNCTION(bool state) {
  if (state) {

    previous = millis();
    while ( ((millis() - previous) < 5000)  && !stringComplete) {
      digitalWrite(TX_LED, HIGH);
      if (rf.available()) {
        while (rf.available() > 0) {
          char inChar = (char)rf.read();
          if (inChar == '\n') {
            stringComplete = true;
          }
          else {
            inputString += inChar;
          }
        }
        previous = millis();
      }
    }


    if (stringComplete) {

      stringComplete = false;
      rf << _BYTE(rt1AddrH) << _BYTE(rt1AddrL) << _BYTE(chan) << "Node [X] - ACK" << " - MSG NUMBER = " << msgc << endl;
      inputString = "";
      digitalWrite(TX_LED, LOW);

    }

  }
}

/*
    //rf << _BYTE(rt1AddrH) << _BYTE(rt1AddrL) << _BYTE(chan) << MESSAGE_1 << endl;

  previous = millis();
  while ( ((millis() - previous) < 20000)  && !stringComplete) {
    if (rf.available()) {
      while (rf.available() > 0) {
        char inChar = (char)rf.read();
        if (inChar == '\n') {
          digitalWrite(RX_LED, HIGH);
          stringComplete = true;
        }
        else {
          inputString += inChar;
        }
      }
      previous = millis();
    }
  }
  if (stringComplete) {
    stringComplete = false;
    rf << _BYTE(rt1AddrH) << _BYTE(rt1AddrL) << _BYTE(chan) << "ACK" << endl;
    inputString = "";
    digitalWrite(RX_LED, LOW);
    delay(2000);
  }
  delay(2000);
*/
