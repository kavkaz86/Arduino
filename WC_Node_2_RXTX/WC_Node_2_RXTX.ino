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
#define RX_LED 10
#define TX_LED 11
#define ROUTE_LED 12
#define PWR_LED 13

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

String  MESSAGE_1 = "01234567890123456789012345678901234567890123456789";
String  MESSAGE_2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool  ACK_MODE = false;

/*
   Variables
*/
char aux[MAX] = "";
char aux2[MAX_SQL] = "";
char c;
int countd = 0;
int j = 0;
int ret;
bool stringComplete = false;
long previous = 0;
String inputString = "";         // a String to hold incoming data
unsigned int msgc = 0;
bool  tx_type = false;


SoftwareSerial  rf(RXD, TXD);

void setup() {
  // put your setup code here, to run once:
  inputString.reserve(200);
  //pinMode(RX_LED, OUTPUT);
  pinMode(TX_LED, OUTPUT);
  //pinMode(ROUTE_LED, OUTPUT);
  pinMode(PWR_LED, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUXD, INPUT_PULLUP);
  rf.begin(9600);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(PWR_LED, HIGH);

  Serial.begin(115200);
  while (!digitalRead(AUXD)) {
    Serial << digitalRead(AUXD) << endl;
    delay(500);
  }

}

void loop() { // run over and over
  if (rf.available()) {
    digitalWrite(RX_LED, HIGH);
    while (rf.available() >  0) {
      char inChar = (char)rf.read();
      if (inChar == '\0') {
        stringComplete = true;
      }
      else {
        inputString += inChar;
      }
    }
    digitalWrite(RX_LED, LOW);
  }

  if (stringComplete) {
    digitalWrite(TX_LED, HIGH);
    if (DEBUG) {
      Serial << inputString << endl; delay(200);
    }
    digitalWrite(ROUTE_LED, HIGH);
    rf << _BYTE(rt3AddrH) << _BYTE(rt3AddrL) << _BYTE(chan) << " " << inputString << '\0'; delay(250);
    while (!digitalRead(AUXD)) {
      Serial << digitalRead(AUXD) << endl;
      delay(25);
    }
    delay(100);
    inputString = "";
    stringComplete = false;
    digitalWrite(TX_LED, LOW);
    digitalWrite(ROUTE_LED, LOW);
  }
}

