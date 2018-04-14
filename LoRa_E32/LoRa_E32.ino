#include  "SoftwareSerial.h"
#include  "Streaming.h"

/*
 * Globals variables
 */
#define M0  A0
#define M1  A1
#define AUX A2
#define TXD 9
#define RXD 10
#define SWT 11

//Initialise the Software Serial Class
SoftwareSerial  lora(RXD,TXD);

//Data variables
String  data_IN = ""; // Radio TX Data
bool  alreadyON = false; // Last state memory 
 
void setup() {

//Setup MCU I/O Pins 
pinMode(LED_BUILTIN, OUTPUT);
pinMode(M0, OUTPUT);
pinMode(M1, OUTPUT);
pinMode(AUX, INPUT);
pinMode(SWT, OUTPUT);

//Configure initial I/O states 
digitalWrite(SWT, HIGH);
alreadyON = true; // toggle the alreadyON flag to indicate the module is already on.
digitalWrite(LED_BUILTIN, HIGH); 

//Open Serial communications - Serial: Debug Port, lora: Radio port
Serial.begin(9600);
lora.begin(9600); 

//Lora module active radio state: M0 - Low, M1 - Low 
digitalWrite(M0, LOW);
digitalWrite(M1, LOW);
delay(250);


}

void loop() {


if(lora.available()){  // Check if Radio has incoming data on the Rx_Pin. 
  delay(200);          // Small delay to buffer data on the UART
  while(lora.available() > 0 ){
    data_IN += (char)lora.read(); //Append the RF-data to the string data_IN
    Serial << data_IN << endl;
  }
  
  if(data_IN == "OFF" && alreadyON){
    digitalWrite(SWT, LOW);
    lora << "PX4 - OFF" << endl;
    alreadyON = false; 
  digitalWrite(LED_BUILTIN, HIGH);
  }
  
  else if(data_IN =="ON" && !alreadyON){
    digitalWrite(SWT, HIGH);
    lora << "PX4 - ON" << endl;
    alreadyON = true;
  }
  data_IN = "";
  
}

}

