

/*
This software was made to demonstrate how to quickly get your Atlas Scientific product running on the Arduino platform.
An Arduino Duemilanove board was used to test this code.
This code was written in the Arudino 1.0 IDE
Modify the code to fit your system.
Code efficacy was NOT considered, this is a demo only.
The soft serial port TX line goes to the RX pin.
The soft serial port RX line goes to the TX pin.
Make sure you also connect to power and GND pins to power and a common ground.
Data is received and re-sent through the Arduinos hardware UART TX line.
Open TOOLS > serial monitor, set the serial monitor to the correct serial port and set the baud rate to 38400.
Remember, select carriage return from the drop down menu next to the baud rate selection; not "both NL & CR".
The data from the Atlas Scientific product will come out on the serial monitor.
Type in a command in the serial monitor and the Atlas Scientific product will respond.
*/



#include <Streaming.h>
#include <SoftwareSerial.h>                                                    //add the soft serial libray
#define rxpin 2                                                                //set the RX pin to pin 2
#define txpin 3                                                                //set the TX pin to pin 3


SoftwareSerial myserial(rxpin, txpin);                                         //enable the soft serial port

int scounter=0;
String inputstring = "";                                                       //a string to hold incoming data from the PC
String sensorstring = "";                                                      //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                                          //have we received all the data from the PC
boolean sensor_stringcomplete = false;                                         //have we received all the data from the Atlas Scientific product


  void setup(){      //set up the hardware
     Serial.begin(9600);                                                      //set baud rate for the hardware serial port to 38400
     myserial.begin(38400);
     inputstring.reserve(5);                                                   //set aside some bytes for receiving data from the PC
     sensorstring.reserve(30);       //set aside some bytes for receiving data from Atlas Scientific product
      myserial.print("e");  delay(100); myserial.print('\r'); delay(100);
     }
 
 
 void loop(){                                                                   //here we go....

  delay(10000);
  myserial.print("c");  delay(100); myserial.print('\r'); delay(100);
  
  while(scounter<25){
    
    while (myserial.available()) {
           char inchar = (char)myserial.read();                                                   
           sensorstring += inchar;               
           if (inchar == '\r') {             
             Serial << scounter << "- " << sensorstring << endl;
             scounter++;    
             sensorstring="";          
            }          
           }          
  }
  myserial.print("e");  delay(100); myserial.print('\r'); delay(100);
  scounter =0;
  
}



