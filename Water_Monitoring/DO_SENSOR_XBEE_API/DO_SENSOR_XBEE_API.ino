#include <XBee.h>
#include <SoftwareSerial.h>                                                    //add the soft serial libray
#include <Streaming.h>


#define rxpin 6                                                                //set the RX pin to pin 2
#define txpin 7                                                                //set the TX pin to pin 3

 
// create the XBee object
XBee xbee = XBee();
SoftwareSerial myserial(rxpin, txpin);                                         //enable the soft serial port

byte payload[20]; 

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

String inputstring = "";                                                       //a string to hold incoming data from the PC
String sensorstring = "";                                                          //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                                          //have we received all the data from the PC
boolean sensor_stringcomplete = false;                                         //have we received all the data from the Atlas Scientific prod



void setup() {


  Serial.begin(38400);
  xbee.setSerial(Serial);
  myserial.begin(38400);                                                    //set baud rate for software serial port to 38400
  inputstring.reserve(5);                                                   //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                                                 //set aside some bytes for receiving data from Atlas Scientific product                                               //set aside some bytes for receiving data from Atlas Scientific product
}

 void serialEvent() {                                                         //if the hardware serial port receives a char
               char inchar = (char)Serial.read();                               //get the char we just received
               inputstring += inchar;                                           //add it to the inputString
               if(inchar == '\r') {input_stringcomplete = true;}                //if the incoming character is a <CR>, set the flag
              }               
              
void loop() {
 

   if (input_stringcomplete){                                                   //if a string from the PC has been recived in its entierty 
      myserial.print(inputstring);                                             //send that string to the Atlas Scientific product
      inputstring = "";                                                        //clear the string:
      input_stringcomplete = false;                                            //reset the flage used to tell if we have recived a completed string from the PC
      }
 

  while (myserial.available()) {                                               //while a char is holding in the serial buffer
         char inchar = (char)myserial.read();                                  //get the new char
         sensorstring += inchar;                                               //add it to the sensorString
         if (inchar == '\r') sensor_stringcomplete = true;                     //if the incoming character is a <CR>, set the flag
        }
 sensorstring.getBytes(payload, sensorstring.length() );        
 xbee.send(zbTx);

   if (sensor_stringcomplete){                                                 //if a string from the Atlas Scientific product has been received in its entirety
       //Serial.print(sensorstring);                                             //use the hardware serial port to send that data to the PC
       sensorstring = "";                                                      //clear the string:
       sensor_stringcomplete = false;                                          //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
      }

 

 
}

