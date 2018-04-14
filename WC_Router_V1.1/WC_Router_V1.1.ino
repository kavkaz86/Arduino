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
uint8_t rt1AddrL = 3;

uint8_t rt3AddrH = 0;
uint8_t rt3AddrL = 1;

uint8_t chan = 34;

/* Node Info Vars */
char  f_id[10];
char  n_addr[10];
char  s_addr_H[10];
char  s_addr_L[10];
char  d_addr_H[10];
char  d_addr_L[10];
char  options[10];
char  rf_data[100];


char aux[MAX] = "";
char aux2[MAX_SQL] = "";
char c;
int countd = 0;
int j = 0;
int ret;


bool stringComplete = false;

SoftwareSerial  rf(RX, TX);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  rf.begin(9600);

  pinMode(RX_LED, OUTPUT);
  pinMode(TX_LED, OUTPUT);
  pinMode(ROUTE_LED, OUTPUT);
  pinMode(ACK_LED, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  RF_mode(0, 0);

}

void  RF_mode(bool m0, bool m1) {

  digitalWrite(M0, m0);
  digitalWrite(M1, m1);
  delay(25);
  while (!digitalRead(AUX));

}

void loop() {

  if (rf.available()) {

    digitalWrite(RX_LED, HIGH);

    c = rf.read();

    if ((c == '\r') || (c == '\0')) {
      //stringComplete = true;
      aux[j] = '_';
    }

    else
    {
      aux[j] = c;
      j++;
    }

    if ((c == '\n') || (j == (MAX - 1))) {

      aux[j] = '\0';
      Serial << '\n' << "------------------------------" << endl;
      Serial << '\n' << "Frame Received = " << aux << endl;

      if (sscanf(aux, "%[^','],%[^','],%[^','],%[^','],%[^',']", &f_id[0], &s_addr_H[0], &d_addr_H[0], &options[0], &rf_data[0]) == 5) {
      //if (sscanf(aux, "%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^',']", &f_id[0], &s_addr_H[0], &s_addr_L[0], &d_addr_H[0], &d_addr_L[0], &options[0], &rf_data[0]) == 7) {
        //RF_mode(0, 0);
        int fid, opt = 0 ;
        fid = atoi(f_id);
        opt = atoi(options);

        if (fid == 0 && opt == 0) {

          ret = 1;
        }
        else if (fid == 0 && opt == 1) {
          //digitalWrite(ACK_LED, HIGH);
          ret = 2;
        }
        else if (fid == 1 && opt == 0) {

          ret = 3;
        }
        else if (fid == 1 && opt == 1) {

          ret = 4;
        }
        else if (fid == 2 && opt == 0) {

          ret = 5;
        }
        else if (fid == 2 && opt == 1) {

          ret = 6;
        }
        else {
          
          ret = -1;
          
        }

        stringComplete = true;


      }

      else if (sscanf(aux, "%[^','],%[^',']", &d_addr_H[0], &f_id[0]) == 2) {
        //RF_mode(0, 0);
        ret = 7 ;
        stringComplete = true;
      }

      else  
        ret = -1;

      switch (ret) {

        case  1: // Store & No ACK
          digitalWrite(TX_LED, HIGH);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << '\n' << "Frame Type = Store & No ACK" << endl; delay(1000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data << '\n' << endl; delay(1000);
          Serial << bsAddrH << "," << bsAddrL << "," << chan << '\n' << "Frame Type = Store & No ACK" << endl; 
          Serial << bsAddrH << "," << bsAddrL << "," << chan << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data << '\n' << endl;
          digitalWrite(TX_LED, LOW);
          break;

        case 2: // Store with ACK
          digitalWrite(TX_LED, HIGH);
          digitalWrite(ACK_LED, HIGH);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << '\n' << "Frame Type = Store with ACK" << endl; delay(1000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data << endl; delay(1000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "ACK Frame: " << "Destination Address: " << s_addr_H  << "," << "f_id: 4" << endl; delay(1000);
          Serial << '\n' << "Frame Type = Store with ACK" << endl;
          Serial << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data << endl;
          Serial << "ACK Frame: " << "Destination Address: " << s_addr_H  << "," << "f_id: 4" << endl;
          digitalWrite(TX_LED, LOW);
          digitalWrite(ACK_LED, LOW);
          break;

        case 3: //Command & No Ack
          digitalWrite(TX_LED, HIGH);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << '\n' << "Frame Type = RF Command & No ACK" << endl; delay(1000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data  << endl; delay(1000);
          Serial << '\n' << "Frame Type = RF Command & No ACK" << endl;
          Serial << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data  << endl;
          digitalWrite(TX_LED, LOW);

          break;

        case 4: //Command with ACK
          digitalWrite(TX_LED, HIGH);
          digitalWrite(ACK_LED, HIGH);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << '\n' << "Frame Type = Command with ACK" << endl; delay(1000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data << endl; delay(1000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "ACK Frame: " << "Destination Address: " << s_addr_H  << "," << "f_id: 4" << endl; delay(1000);
          Serial << '\n' << "Frame Type = Command with ACK" << endl;
          Serial << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data << endl;
          Serial << "ACK Frame: " << "Destination Address: " << s_addr_H  << "," << "f_id: 4" << endl;
          digitalWrite(TX_LED, LOW);
          digitalWrite(ACK_LED, LOW);
          break;

        case 5: //Route & no ACK
          digitalWrite(TX_LED, HIGH);
          digitalWrite(ROUTE_LED, HIGH);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << '\n' << "Frame Type = Route & No ACK" << endl; delay(1000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' <<  "RF - Data: " << rf_data << endl; delay(1000);
          Serial << '\n' << "Frame Type = Route & No ACK" << endl;
          Serial << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << _DEC(options) << '\n' << "RF - Data: " << rf_data << endl;
          digitalWrite(ROUTE_LED, LOW);
          digitalWrite(TX_LED, LOW);
          break;

        case 6: //Route with ACK
          digitalWrite(TX_LED, HIGH);
          digitalWrite(ACK_LED, HIGH);
          digitalWrite(ROUTE_LED, HIGH);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << '\n' << "Frame Type = Route with ACK" << endl; delay(5000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n'  << "RF - Data: " << rf_data  << endl; delay(5000);
          rf << _BYTE(bsAddrH) <<  _BYTE(bsAddrL) << _BYTE(chan) << "ACK Frame: " << "Destination Address: " << s_addr_H  << "," << "f_id: 4" << endl; delay(250);
          Serial << '\n' << "Frame Type = Route with ACK" << endl;
          Serial << "Frame_id: " << f_id << '\n' << "Source Address: " << s_addr_H << '\n' << "Destination Address: " << d_addr_H << '\n' << "Options: " << options << '\n' << "RF - Data: " << rf_data  << endl;
          Serial << "ACK Frame: " << "Destination Address: " << s_addr_H  << "," << "f_id: 4" << endl;
          digitalWrite(ROUTE_LED, LOW);
          digitalWrite(TX_LED, LOW);
          digitalWrite(ACK_LED, LOW);
          break;

        case 7:
          digitalWrite(ACK_LED, HIGH);
          Serial << '\n' << "Frame Type = ACK" << endl;
          digitalWrite(ACK_LED, LOW);
          break;

        case -1:

          Serial << '\n' << "Frame = " << aux << endl;
          Serial << "Bad Frame Received" << endl;
          break;
      }

      j = 0;
      aux[j] = '\0';
      stringComplete = false;
      Serial  << '\n' <<  "------------------------------" << endl;
      digitalWrite(RX_LED, LOW);
    }
  }
}

/*if (sscanf(aux, "%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^',']", &f_id[0], &s_addr_H[0], &s_addr_L[0], &d_addr_H[0], &d_addr_L[0], &options[0], &rf_data[0]) == 7) {

        int fid, opt = 0 ;
        fid = atoi(f_id);
        opt = atoi(options);

        if (fid == 0 && opt == 0) {
          ret = 1;
        }
        else if (fid == 0 && opt == 1) {
          ret = 2;
        }
        else if (fid == 1 && opt == 0) {
          ret = 3;
        }
        else if (fid == 1 && opt == 1) {
          ret = 4;
        }
        else if (fid == 2 && opt == 0) {
          ret = 5;
        }
        else if (fid == 2 && opt == 1) {
          ret = 6;
        }
        else {
          ret = -1;
        }

        stringComplete = true;

      } */

