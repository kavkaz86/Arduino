#include  "SoftwareSerial.h"
#include  "Streaming.h"

#define LED_TX  4
#define M0  5
#define M1  6
#define TXD 7
#define RXD 8
#define AUX 9
#define LED_PWR 10


SoftwareSerial  lora(RXD,TXD);
String  data_IN = "";
 
void setup() {

pinMode(M0, OUTPUT);
pinMode(M1, OUTPUT);
pinMode(AUX, INPUT);
pinMode(LED_PWR, OUTPUT);
pinMode(LED_TX, OUTPUT);

Serial.begin(9600);
lora.begin(9600); 

digitalWrite(LED_PWR, HIGH);
digitalWrite(M0, LOW);
digitalWrite(M1, LOW);
delay(250);


}
int i = 0 ;
void loop() {

  digitalWrite(LED_TX, HIGH);
  
  delay(1000);

  lora << "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" << endl; 

  delay(2000); 

  digitalWrite(LED_TX, LOW);

  delay(2000); 

}
