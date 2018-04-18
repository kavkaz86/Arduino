/*
XBee RX test for a Arduino Mega2560 using Serial3 as the XBee serial
input for a Series 2 XBee.  This is NOT based on the examples that come with
the Arduino XBee library.

See, the examples there and most other places on the web SUCK.  Andrew's
library is much easier to use than the illustrations would lead you to believe.

This is a HEAVILY commented example of how send a text packet using series 2
XBees.  Series 1 XBees are left as an exercise for the student.
*/

#include <XBee.h>

XBee xbee = XBee();
String sal ="23,23,34,";
String oxygen = "100,12,";
String ph = "6.00"; 
String tx = "";
// This is the XBee broadcast address.  You can use the address
// of any device you have also.
XBeeAddress64 coordinator = XBeeAddress64(0x00000000, 0x0000FFFF);

//char Hello[] = "Hello World\r";
char Buffer[128];  // this needs to be longer than your longest packet.

void setup() {
  // start serial
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); // Disable radio sleep
  
  delay(2000); 
  
  Serial.begin(115200);
  // and the software serial port
  //Serial3.begin(9600);
  // now that they are started, hook the XBee into
  // Software Serial
  xbee.setSerial(Serial);
  //Serial.println("Initialization all done!");
  tx.reserve(100);
  
}

void loop() {
  char str[100]; 
  tx = sal + oxygen + ph + '\r'; 
  tx.toCharArray(str, 128); 
  ZBTxRequest zbtx = ZBTxRequest(coordinator, (uint8_t *)str, strlen(str));
  xbee.send(zbtx);
  delay(5000);
  strcpy(Buffer,"I saw what you did last night.\r");
  zbtx = ZBTxRequest(coordinator, (uint8_t *)Buffer, strlen(Buffer));
  xbee.send(zbtx);
  delay(5000);
}
