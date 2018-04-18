#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <OneWire.h>
#include <stdlib.h> //dtostrf(FLOAT,WIDTH,PRECSISION,BUFFER);

#define  UVmsg  "UV= "
#define  TURmsg  "T="
#define  SUmsg  "SU= "
#define  tempmsg  "Temp(C)= "
#define  errmsg  "Error"
#define  smsg  "Success"
#define  clientmsg  "new client"
#define  clientdc  "client disonnected"  
#define  html1  "HTTP/1.1 200 OK"
#define  html2  "Content-Type: text/html"
#define  html3  "Connection: close"
#define  html4  "Refresh: 5"
#define  html5  "<!DOCTYPE HTML>"
#define  html6  "<html>"
#define  html7  "</html>"  
#define  newline  "<br />" 

OneWire  ds(9);  // on pin 9

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(139,230,147,4);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

EthernetClient client;

const int chipSelect = 4;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(38400);

  pinMode(A0, INPUT); 
  pinMode(A1, INPUT);
  pinMode(A2, INPUT); 

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  //Serial.print("server is at ");
  Serial.println(Ethernet.localIP());


  if (!SD.begin(chipSelect)) {
    Serial.println(errmsg);
    // don't do anything more:
    return;
  }
  else
    Serial.println(smsg);

}


void loop() {
  // listen for incoming clients
  client = server.available();
  if (client) { 
    Serial.println(clientmsg);
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header

          client.println(html1);
          client.println(html2);
          client.println(html3);  // the connection will be closed after completion of the response
          client.println(html4);  // refresh the page automatically every 5 sec
          client.println();
          client.println(html5);
          client.println(html6); 


          // output the value of each analog input pin

          for(int i=0; i < 4 ; i++){

            switch(i){

            case 0:  
              getUV(); 
              break;

            case 1:
              getTurbidity(); 
              break;

            case 2:
              getTemperature(); 
              break;

            case 3:
              getQuantum(); 
              break;  

            default:
              break;  
            }
          }
          client.println(html7);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println(clientdc);
  }
}

void getUV(){

  int rawUV; 
  float milliVolts; 
  float UV;

  analogReference(INTERNAL);
  delay(200);
  rawUV = analogRead(A0);
  
  milliVolts = ((float(rawUV) * 1100)/ 1023); 

  UV = milliVolts * 5;
  
  saveSD(0, UV);
  
  htmlprint(0, UV);

}


void getTurbidity(){

  int rawSens; 
  float turVolt;
  //String dataString = "";

  analogReference(DEFAULT);
  delay(200);
  rawSens = analogRead(A1);

  turVolt = ((float(rawSens) * 5)/ 1023);
  saveSD(1, turVolt);

  htmlprint(1, turVolt); 

} 

void getQuantum(){

  int rawSU; 
  float milliVolts; 
  float quantumSU;

  analogReference(INTERNAL);
  delay(200);
  rawSU = analogRead(A2); 

  milliVolts = ((float(rawSU) * 1100)/ 1023); 

  quantumSU = milliVolts * 5;
  saveSD(2, quantumSU);

  htmlprint(2, quantumSU); 
}

void getTemperature(){

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return;
  }
  //Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
  case 0x10:
    type_s = 1;
    break;
  case 0x28:
    type_s = 0;
    break;
  case 0x22:
    type_s = 0;
    break;
  default:
    return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }


  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  saveSD(3, celsius);
  htmlprint(3, celsius); 

}

void htmlprint(int i, float sens){

  switch(i){

  case 0:
    client.print(UVmsg);    
    break;

  case 1:
    client.print(TURmsg);
    break;

  case 2:
    client.print(SUmsg);
    break;

  case 3:
    client.print(tempmsg);
    break;

  default:
    break; 

  }
  client.print(sens);
  client.println(newline);
}  


void saveSD(int i, float sens){
 
 char dataString[10]; 
 File dataFile;
 
 switch(i){
 
 case 0:
 
   dataFile = SD.open("UV.txt", FILE_WRITE);
   if (dataFile) {
   dataFile.println(dtostrf(sens,5,2,dataString));
   dataFile.close();
   Serial.println(smsg);
   }
   else {
   Serial.println(errmsg);
   }
 break;
 
 case 1:
 
   dataFile = SD.open("turb.txt", FILE_WRITE);
   if (dataFile) {
   dataFile.println(dtostrf(sens,5,2,dataString));
   dataFile.close();
   Serial.println(smsg);
   }
   else {
   Serial.println(errmsg);
   }
 break;
 
 case 2:
 
   dataFile = SD.open("SU.txt", FILE_WRITE);
   if (dataFile) {
   dataFile.println(dtostrf(sens,5,2,dataString));
   dataFile.close();
   Serial.println(smsg);
   }
   else {
   Serial.println(errmsg);
   }
 break; 
 
 case 3:
 
   dataFile = SD.open("temp.txt", FILE_WRITE);
   if (dataFile) {
   dataFile.println(dtostrf(sens,5,2,dataString));
   dataFile.close();
   Serial.println(smsg);
   }
   else {
   Serial.println(errmsg);
 }
 break;
 
 default:
 break;      
 }
 }






