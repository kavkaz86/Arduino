#include "arduino4G.h"
#include  "Streaming.h"
#include  "SoftwareSerial.h"

/*
   RF Module
*/
//#define M0  5
//#define M1  6

#define TXD  5
#define RXD  4
#define AUXD  6
uint8_t chan = 34;
uint8_t rt26AddrH = 0;
uint8_t rt26ddrL = 26;


// variables
uint8_t connection_status;
char operator_name[16];
uint8_t error;
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete


//////////////////////////////////////////////////
char phone_number[] = "+61466545443";
char sms_body[] = "";
//////////////////////////////////////////////////

// APN settings
///////////////////////////////////////
char apn[] = "telstra.internet";
char login[] = "";
char password[] = "";
///////////////////////////////////////

SoftwareSerial  rf(RXD, TXD);

void setup()
{
  inputString.reserve(200);
  pinMode(AUXD, INPUT_PULLUP);
  rf.begin(9600);

  delay(50);

  error = _4G.ON();

  _4G.set_APN(apn, login, password);

  //////////////////////////////////////////////////
  // 2. Show APN settings via USB port
  //////////////////////////////////////////////////
  _4G.show_APN();


  if (error == 0)
  {
    Serial.println(F("1. 4G module ready"));

    ////////////////////////////////////////////////
    // 1.1. Check connection to network and continue
    ////////////////////////////////////////////////
    connection_status = _4G.checkDataConnection(30);
    if (connection_status == 0)
    {
      Serial.println(F("1.1. Module connected to network"));

      //////////////////////////////////////////////
      // 1.2. Get RSSI
      //////////////////////////////////////////////
      error = _4G.getRSSI();
      if (error == 0)
      {
        Serial.print(F("1.2. RSSI: "));
        Serial.print(_4G._rssi, DEC);
        Serial.println(F(" dBm"));
      }
      else
      {
        Serial.println(F("1.2. Error calling 'getRSSI' function"));
      }

      //////////////////////////////////////////////
      // 1.3. Get Network Type
      //////////////////////////////////////////////
      error = _4G.getNetworkType();

      if (error == 0)
      {
        Serial.print(F("1.3. Network type: "));
        switch (_4G._networkType)
        {
          case 0:
            Serial.println(F("GPRS"));
            break;
          case 1:
            Serial.println(F("EGPRS"));
            break;
          case 2:
            Serial.println(F("WCDMA"));
            break;
          case 3:
            Serial.println(F("HSDPA"));
            break;
          case 4:
            Serial.println(F("LTE"));
            break;
          case 5:
            Serial.println(F("Unknown or not registered"));
            break;
        }
      }
      else
      {
        Serial.println(F("1.3. Error calling 'getNetworkType' function"));
      }

      //////////////////////////////////////////////
      // 1.4. Get Operator name
      //////////////////////////////////////////////
      memset(operator_name, '\0', sizeof(operator_name));
      error = _4G.getOperator(operator_name);

      if (error == 0)
      {
        Serial.print(F("1.4. Opertor: "));
        Serial.println(operator_name);
      }
      else
      {
        Serial.println(F("1.4. Error calling 'getOpertor' function"));
      }

      //////////////////////////////////////////////
      // 1.5. Show RTC time from Network
      //////////////////////////////////////////////
      Serial.println(F("1.5. RTC Network Time: "));

      error = _4G.showTimeFrom4G();

      if (error == 0)
      {
        Serial.println(F("1.5. Time Show OK"));
      }
      else
      {
        Serial.println(F("1.5. Error calling 'showTimeFrom4G' function"));
      }
    }
  }
  else
  {
    // Problem with the communication with the 4G module
    Serial.println(F("4G module not started"));
    Serial.print(F("Error code: "));
    Serial.println(error, DEC);
  }
  _4G.OFF();

  //Serial.begin(115200);
  //Serial << "Shutting Down 4G Module: ";
  //for(int i=0; i<30; i++){
  // Serial << i;
  // delay(1000);
  //}
  //delay(30000);
  //Serial << endl;
  //Serial << F("Program Started") << endl;
  //rf.listen();
  //Serial.begin(115200);
  //delay(200);
  //if (!rf.listen()) {
  //Serial << "Error - Unable to start RF port" << endl;
  //}
  //else {
  //Serial << "Success - Listening on RF Port" << endl;
  //rf.flush(); // DEBUG
  //}
  //Serial.end();
  stringComplete = false;
  
}

void loop()
{
  if (rf.available()) {
    delay(50);
    while (rf.available() > 0 ) {

      char inChar = (char)rf.read();
      // add it to the inputString:

      inputString += inChar;
      // if the incoming character is a newline, set a flag so the main loop can
      // do something about it:
      if (inChar == '\n') {
        stringComplete = true;
      }
    }
  }

  if (stringComplete) {

    //Serial << F("Message Arrived") << endl;
    inputString.trim();
    //delay(1000);
    Serial.begin(115200);
    delay(200);
    Serial << endl;
    if (!rf.stopListening()) {
      Serial << "Error - RF port still listening" << endl;
    }
    else {
      Serial << "Success - Stopped Listeing to RF Port" << endl;
    }

    Serial << F("Message Arrived") << inputString << endl;
    delay(50);
    Serial.end();
    delay(200);

    error = _4G.ON();


    if (error == 0)
    {
      Serial.println(F("2.1. 4G module Ready"));
    }
    connection_status = _4G.checkDataConnection(120);
    if (connection_status == 0)
    {

      error = _4G.getRSSI();
      if (error == 0)
      {
        Serial.print(F("1.2. RSSI: "));
        Serial.print(_4G._rssi, DEC);
        Serial.println(F(" dBm"));
      }
      else
      {
        Serial.println(F("1.2. Error calling 'getRSSI' function"));
      }

      //////////////////////////////////////////////
      // 1.3. Get Network Type
      //////////////////////////////////////////////
      error = _4G.getNetworkType();

      if (error == 0)
      {
        Serial.print(F("1.3. Network type: "));
        switch (_4G._networkType)
        {
          case 0:
            Serial.println(F("GPRS"));
            break;
          case 1:
            Serial.println(F("EGPRS"));
            break;
          case 2:
            Serial.println(F("WCDMA"));
            break;
          case 3:
            Serial.println(F("HSDPA"));
            break;
          case 4:
            Serial.println(F("LTE"));
            break;
          case 5:
            Serial.println(F("Unknown or not registered"));
            break;
        }
      }
      else
      {
        Serial.println(F("1.3. Error calling 'getNetworkType' function"));
      }

      //////////////////////////////////////////////
      // 1.4. Get Operator name
      //////////////////////////////////////////////
      memset(operator_name, '\0', sizeof(operator_name));
      error = _4G.getOperator(operator_name);

      if (error == 0)
      {
        Serial.print(F("1.4. Opertor: "));
        Serial.println(operator_name);
      }
      else
      {
        Serial.println(F("1.4. Error calling 'getOpertor' function"));
      }

      //////////////////////////////////////////////
      // 1.5. Show RTC time from Network
      //////////////////////////////////////////////
      Serial.println(F("1.5. RTC Network Time: "));

      error = _4G.showTimeFrom4G();

      if (error == 0)
      {
        Serial.println(F("1.5. Time Show OK"));
      }
      else
      {
        Serial.println(F("1.5. Error calling 'showTimeFrom4G' function"));
      }


      error = _4G.configureSMS();

      if (error == 0)
      {
        Serial.println(F("2.1. 4G module configured to use SMS"));
      }
      else
      {
        Serial.print(F("2.1. Error calling 'configureSMS' function. Code: "));
        Serial.println(error, DEC);
      }

      ////////////////////////////////////////////////
      // 3. Send SMS
      ////////////////////////////////////////////////
      uint8_t inputSize = inputString.length() + 1 ;
      char charBuf[inputSize];
      inputString.toCharArray(charBuf, inputSize);
      Serial.println(F("4.2. Sending SMS..."));
      delay(10000);
      error = _4G.sendSMS(phone_number, charBuf);

      if (error == 0)
      {
        Serial.println(F(" done"));
      }
      else
      {
        Serial.print(F("error. Code: "));
        Serial.println(error, DEC);
      }
      // clear the string:

      _4G.OFF();
      //delay(30000);
    }

    inputString = "";
    stringComplete = false;

    Serial.begin(115200);
    delay(200);
    if (!rf.listen()) {
      Serial << "Error - Unable to start RF port" << endl; delay(200);
    }
    else {
      Serial << "Success - Listening on RF Port" << endl; delay(200);
      rf.flush(); // DEBUG
      delay(200);
    }   
    Serial.end();
    delay(200);

    //_4G.beginUART(); */
  }
}

