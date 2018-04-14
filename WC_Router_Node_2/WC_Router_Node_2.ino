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
#define AUX 9


/*
   NWK Parameters
*/
uint8_t bsAddrH = 0;
uint8_t bsAddrL = 0;

uint8_t rt1AddrH =  0;
uint8_t rt1AddrL = 1;

uint8_t rt2AddrH =  0;
uint8_t rt2AddrL = 2;

uint8_t rt3AddrH = 0;
uint8_t rt3AddrL = 3;

uint8_t chan = 34;


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

SoftwareSerial  rf(RXD, TXD);

void setup() {
  // put your setup code here, to run once:
  inputString.reserve(200);
  pinMode(RX_LED, OUTPUT);
  pinMode(TX_LED, OUTPUT);
  pinMode(ROUTE_LED, OUTPUT);
  pinMode(PWR_LED, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(PWR_LED, HIGH);

  rf.begin(9600);
  Serial.begin(115200);



}

void loop() {

  rf << _BYTE(rt1AddrH) << _BYTE(rt1AddrL) << _BYTE(chan) << "HELLO MOTHERFUCKER" << endl;

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
}
