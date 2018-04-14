#include  "Streaming.h"

int delim_count = 0;
int counta = 0;
bool  stringComplete = false;
int delim_pos[20];
String  rx_in = "";
int countb = 0;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  rx_in.reserve(200);

}

void loop() {

  if (stringComplete) {

    Serial << "InputString = " << rx_in << ", Number of Delim = " << delim_count << ", Size of Input = " << rx_in.length() << endl;

    for(int i=0; i < delim_count; i++){
      Serial << "Delimiter Position " << i << " = " <<  delim_pos[i] << endl;
    }
    stringComplete = false;
    delim_count = 0;
    countb = 0;
    counta = 0;
    rx_in = "";
  }


}

void  serialEvent() {

  if (Serial.available()) {
    //delay(200);

    char  inChar = (char)Serial.read();
    //Serial << inChar; 
    rx_in += inChar;

    if (inChar == '\r') {
      stringComplete = true;
    }
    if (inChar == '|') {
      delim_count++;
      delim_pos[countb] = counta;
      countb++;
    }
    
    counta++;
  }
}

