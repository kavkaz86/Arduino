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
#define ACK_LED 13

/*
   RF Module
*/
#define M0  5
#define M1  6
#define TX  7
#define RX  8
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

char aux[MAX] = "";
char aux2[MAX_SQL] = "";
char c;
int countd = 0;
int j = 0;
int ret;


bool stringComplete = false;

long previous = 0;
String inputString = "";         // a String to hold incoming data

SoftwareSerial  rf(RX, TX);

void setup() {
  pinMode(RX_LED, OUTPUT);
  pinMode(TX_LED, OUTPUT);
  pinMode(ROUTE_LED, OUTPUT);
  pinMode(ACK_LED, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);
  //RF_mode(0, 0);
  
  Serial.begin(115200);
  rf.begin(9600);

 digitalWrite(M0, LOW);
 digitalWrite(M1, LOW);
  RF_mode(0,0);
  inputString.reserve(200);

  digitalWrite(ROUTE_LED, HIGH); 

}

void  RF_mode(bool m0, bool m1) {

  digitalWrite(M0, m0);
  digitalWrite(M1, m1);
  delay(25);
  while(!digitalRead(AUX));

}

void loop() {
  Serial << _BYTE(rt2AddrH) << _BYTE(rt2AddrL) << _BYTE(chan) << "HELLO MOTHERFUCKER" << endl;
  rf << _BYTE(rt2AddrH) << _BYTE(rt2AddrL) << _BYTE(chan) << "HELLO MOTHERFUCKER" << endl;
 
  
  while ( (millis() - previous) < 20000 ) {

    if (rf.available()) {
      digitalWrite(RX_LED, HIGH);
      delay(20);

      while (rf.available() > 0) {
        char inChar = (char)rf.read();
        inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
          stringComplete = true;
        }
      }
      previous= millis(); 
    }
  }
  if(stringComplete){
    rf << _BYTE(rt1AddrH) << _BYTE(rt1AddrL) << _BYTE(chan) << "ACK" << endl;
    inputString = "";
    stringComplete = false;
    digitalWrite(RX_LED, LOW);
  previous = millis();
  }
  delay(20); 
}
