#include  "Streaming.h"
#include  "SoftwareSerial.h"
#include "LowPower.h"


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

uint8_t nid[] = {'0', '0', //0 - [0][1]
                 '0', '1', //1 - [2][3]
                 '0', '2', //2 - [4][5]
                 '0', '3', //3 - [6][7]
                 '0', '4', //4 - [8][9]
                 '0', '5', //5 - [10][11]
                 '0', '6', //6 - [12][13]
                 '0', '7', //7 - [14][15]
                 '0', '8', //8 - [16][17]
                 '0', '9', //9 - [18][19]
                 '0', 'A', //10 - [20][21]
                 '0', 'B', //11 - [22][23]
                 '0', 'C', //12 - [24][25]
                 '0', 'D', //13 - [26][27]
                 '0', 'E', //14 - [28][29]
                 '0', 'F', //15 - [30][31]
                 '1', '0', //16 - [32][33]
                 '1', '1', //17 - [34][35]
                 '1', '2', //18 - [36][37]
                 '1', '3', //19 - [38][39]
                 '1', '4', //20 - [40][41]
                 '1', '5', //21 - [42][43]
                 '1', '6', //22 - [44][45]
                 '1', '7', //23 - [46][47]
                 '1', '8', //24 - [48][49]
                 '1', '9', //25 - [50][51]
                 '1', 'A', //26 - [52][53]
                 '1', 'B', //27 - [54][55]
                 '1', 'C', //28 - [56][57]
                 '1', 'D', //29 - [58][59]
                 '1', 'E'  //30 - [60][61]
                };

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

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUXD, INPUT_PULLUP);
  Serial.begin(115200);
  rf.begin(9600);
  
  while (!digitalRead(AUXD)) {
    Serial << digitalRead(AUXD) << endl;
    delay(25);
  }

}

void  RF_mode(bool m0, bool m1) {

  digitalWrite(M0, m0);
  digitalWrite(M1, m1);
  while(!digitalRead(AUXD));
  delay(250);

}

void loop() {

  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF); delay(100);

  msgc++;
  
  RF_mode(0,0);
  
  if (msgc % 2 == 0) {

    rf << _BYTE(nid[0]) << _BYTE(nid[1]) << _BYTE(chan) << MESSAGE_1 << '\0'; delay(250);

    while (!digitalRead(AUXD));
    
  }

  else {

    rf << _BYTE(nid[0]) << _BYTE(nid[1]) << _BYTE(chan) << MESSAGE_2 << '\0'; delay(250);

    while (!digitalRead(AUXD));

  }

  RF_mode(1,1);

}

/*
 * BEGIN
 * 
 * TRANSMISSION DATA // Software Serial
 * 
 * SAVE TO SD CARD. // SD Library
 * 
 * SLEEP // Low Power Library
 * 
 */
