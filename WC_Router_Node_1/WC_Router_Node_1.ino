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
#define LED_TX  4
#define LED_PWR 10

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
  pinMode(LED_PWR, OUTPUT);
  pinMode(LED_TX, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(LED_PWR, HIGH);

  rf.begin(9600);
  Serial.begin(115200);



}

void loop() {

  if (rf.available()) {
    while (rf.available() > 0) {
      char inChar = (char)rf.read();    
      if (inChar == '\n') {
        digitalWrite(LED_TX, HIGH);
        stringComplete = true;
      } 
      else{
        inputString += inChar;   
      }
    }
  }
  if (stringComplete) {
    stringComplete = false;
    if(inputString.startsWith("HELLO MOTHERFUCKER")){
      Serial << "Transmission Count = " << inputString << endl;
      rf << _BYTE(rt2AddrH) << _BYTE(rt2AddrL) << _BYTE(chan) << "ACK" << endl;
    }
    inputString = "";
    digitalWrite(LED_TX, LOW);
  }
}
