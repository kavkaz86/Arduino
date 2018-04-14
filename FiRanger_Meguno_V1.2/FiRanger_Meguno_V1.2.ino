#include  "Streaming.h"
#include  "SoftwareSerial.h"
#include "MegunoLink.h"
#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 8
#define XBEE_RXD 10
#define XBEE_TXD 11

char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

SoftwareSerial  xb(XBEE_RXD, XBEE_TXD);


Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
uint8_t type;
String inputString = "";         // a String to hold incoming data
bool  stringComplete = false;
int err = 0;

int delim_count = 0;
int counta = 0;

int delim_pos[20];
String  rx_in = "";
int countb = 0;
String  data_out[20];

String  LONGITUDE = "-31.752713";
String  LATITUDE = "115.772338";
String nodeId = "1";

void setup() {
  // put your setup code here, to run once:
  rx_in.reserve(200);
  Serial.begin(115200);
  UI_set();


  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  fonaSS.listen();
  if (! fona.begin(*fonaSerial));
  delay(15000);
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  fona.setGPRSNetworkSettings(F("telstra.wap"), F(""), F(""));
  //delay(1000);

  uint8_t n = fona.getRSSI();
  int8_t r;

  Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30)) {
    r = map(n, 2, 30, -110, -54);
  }
  Serial.print(r); Serial.println(F(" dBm"));

  n = fona.getNetworkStatus();
  Serial.print(F("Network status "));
  Serial.print(n);
  Serial.print(F(": "));
  if (n == 0) Serial.println(F("Not registered"));
  if (n == 1) Serial.println(F("Registered (home)"));
  if (n == 2) Serial.println(F("Not registered (searching)"));
  if (n == 3) Serial.println(F("Denied"));
  if (n == 4) Serial.println(F("Unknown"));
  if (n == 5) Serial.println(F("Registered roaming"));

  fonaSS.stopListening();
  delay(100);
  xb.begin(38400);
  xb.listen();
  delay(100);

  Serial << F("(Listening on XBee Port") << endl;

}


void loop() {

  if (xb.available()) {

    char  inChar = (char)xb.read();

    rx_in += inChar;

    if (inChar == '\r') {
      stringComplete = true;
    }
    if (inChar == ',') {
      delim_count++;
      delim_pos[countb] = counta;
      countb++;
    }

    counta++;
  }


  /*
     Reporting Frame: fID, nID, IR1, IR2, IR3, IR4, Dust_V, Dust_ppm, Gas_V  - 8 delims
     Dust Frame:  fID, nID, Dust_ID, Dust_V, Dust_ppm   - 4 delims
     Primary IR Detection:  fID, nID, IR_MAP, IR_1, IR_2, IR_3, IR_4  - 6 delims
     Gas Frame: fID, nID, GasID, Gas_V - 3 delims
  */

  if (stringComplete) {

    rx_in.trim();
    
    //Send SMS Routine.
    Serial << "InputString = " << rx_in << " | Number of Delim = " << delim_count << " | Size of Input = " << rx_in.length() << endl;

    for (int i = 0; i < delim_count; i++) {
      Serial << "Delimiter Position " << i << " = " <<  delim_pos[i] << endl;
    }

    if (!xb.stopListening()) {
      Serial << F("Error: Xbee still listening") << endl;
    }
    else {
      Serial << F("Stopped Listeing to XBee Port") << endl;
    }
    delay(1000);
    if (!fonaSS.listen()) {
      Serial << F("Error: Unable to connect to FONA Serial") << endl;
    }
    else {
      Serial << F("Listening on Fona Port") << endl;
    }
    delay(1000);

    Serial << F("---> Sending data to mobile number 0466545443") << endl;

    uint8_t inputSize = rx_in.length();
    char charBuf[inputSize];
    rx_in.toCharArray(charBuf, inputSize);
    delay(100);
    if (!fona.sendSMS("0466545443", charBuf)) {
      Serial.println(F("Failed to send! - Check 3G/GPRS Connection"));
    } else {
      Serial.println(F("Sent!"));
    }

    int nID = 0;
    float gas_raw = 0;
    float IR1, IR2, IR3, IR4 = 0;
    float dust_raw, dust_ppm = 0;

    switch (delim_count) {

      case  3:  //Gas Sensor Frame Table

        data_out[0] = rx_in.substring(0, delim_pos[0]);
        data_out[1] = rx_in.substring(delim_pos[0] + 1, delim_pos[1]);
        data_out[2] = rx_in.substring(delim_pos[1] + 1, delim_pos[2]);
        data_out[3] = rx_in.substring(delim_pos[2] + 1);

        Serial << F("Gas Sensor Frame - ") << F("FID = ") << data_out[0] << F(" , NID = ") << data_out[1] << F(" , Gas ID = ")
               << data_out[2] <<  F(" , Sensor Voltage = ") << data_out[3] << endl;

        nID = data_out[1].toInt();
        gas_raw = data_out[3].toFloat();

        Serial << F("{TABLE:G_Node_1|SET|Node Address|") << data_out[1] << F("}") << endl;
        Serial << F("{TABLE:G_Node_1|SET|Frame ID|") << data_out[0] << F("}") << endl;
        Serial << F("{TABLE:G_Node_1|SET|Gas-ID|") << data_out[2] << F("}") << endl;
        Serial << F("{TABLE:G_Node_1|SET|Gas-RAW|") << gas_raw << F("}") << endl;
        if (gas_raw > 3.5) {
          Serial << F("{TABLE:G_Node_1|SET|Gas-Rating|2}") << endl;
        }
        else if (gas_raw > 2.0 && gas_raw < 3.5) {
          Serial << F("{TABLE:G_Node_1|SET|Gas-Rating|1}")  << endl;
        }
        else
          Serial << F("{TABLE:G_Node_1|SET|Gas-Rating|0}") << endl;

        Serial << F("{TIMEPLOT:G_TX|DATA|G.N2:r-s4|T|") << gas_raw << "}" << endl;

        break;

      case  4: // Dust Frame:  fID, nID, Dust_ID, Dust_V, Dust_ppm

        data_out[0] = rx_in.substring(0, delim_pos[0]);
        data_out[1] = rx_in.substring(delim_pos[0] + 1, delim_pos[1]);
        data_out[2] = rx_in.substring(delim_pos[1] + 1, delim_pos[2]);
        data_out[3] = rx_in.substring(delim_pos[2] + 1, delim_pos[3]);
        data_out[4] = rx_in.substring(delim_pos[3] + 1);
        Serial << F("Dust Sensor Frame - ") <<  F("FID = ") << data_out[0] << F(" , NID = ") << data_out[1] << F(" , Sensor Map = ") << data_out[2]
               << F(" , Sensor Voltage= ") << data_out[3] << F(" , Dust Concentration (ppm) = ") << data_out[4] << endl;

        nID = data_out[1].toInt();
        dust_raw = data_out[3].toFloat();
        dust_ppm = data_out[4].toFloat();

        Serial << F("{TABLE:D_Node_1|SET|Node Address|") << data_out[1] << F("}") << endl;
        Serial << F("{TABLE:D_Node_1|SET|Frame ID|") << data_out[0] << F("}") << endl;
        Serial << F("{TABLE:D_Node_1|SET|Dust-ID|") << data_out[2] << F("}") << endl;
        Serial << F("{TABLE:D_Node_1|SET|Dust-RAW|") << dust_raw << F("}") << endl;
        Serial << F("{TABLE:D_Node_1|SET|Dust-ppm|") << dust_ppm << F("}") << endl;
        Serial << F("{TIMEPLOT:DRaw_TX|DATA|DRaw.N1:r-s4|T|") << dust_raw << "}" << endl;
        Serial << F("{TIMEPLOT:Dppm_TX|DATA|Dppm.N2:r-s4|T|") << gas_raw << "}" << endl;
        break;

      case  6:  //Primary IR Detection:  fID, nID, IR_MAP, IR_1, IR_2, IR_3, IR_4

        data_out[0] = rx_in.substring(0, delim_pos[0]);
        data_out[1] = rx_in.substring(delim_pos[0] + 1, delim_pos[1]);
        data_out[2] = rx_in.substring(delim_pos[1] + 1, delim_pos[2]);
        data_out[3] = rx_in.substring(delim_pos[2] + 1, delim_pos[3]);
        data_out[4] = rx_in.substring(delim_pos[3] + 1, delim_pos[4]);
        data_out[5] = rx_in.substring(delim_pos[4] + 1, delim_pos[5]);
        data_out[6] = rx_in.substring(delim_pos[5] + 1);
        Serial << F("Thermal Detection Frame - ") <<  F("FID = ") << data_out[0] << F(" , NID = ") << data_out[1] << F(" , Sensor Map = ")
               << data_out[2] <<  F(" , IR_1 = ") << data_out[3] << F(" , IR_2 = ") << data_out[4] << F(" , IR_3 = ")
               << data_out[5] << F(" , IR_4 = ") << data_out[6] << endl;

        nID = data_out[1].toInt();
        IR1 = data_out[3].toFloat();
        IR2 = data_out[4].toFloat();
        IR3 = data_out[5].toFloat();
        IR4 = data_out[6].toFloat();

        Serial << F("{TABLE:IR_Node_1|SET|Node Address|") << data_out[1] << F("}") << endl;
        Serial << F("{TABLE:IR_Node_1|SET|Frame ID|") << data_out[0] << F("}") << endl;
        Serial << F("{TABLE:IR_Node_1|SET|IR-Map|") << data_out[2] << F("}") << endl;
        Serial << F("{TABLE:IR_Node_1|SET|IR-1|") << IR1 << F("}") << endl;
        Serial << F("{TABLE:IR_Node_1|SET|IR-2|") << IR2 << F("}") << endl;
        Serial << F("{TABLE:IR_Node_1|SET|IR-3|") << IR3 << F("}") << endl;
        Serial << F("{TABLE:IR_Node_1|SET|IR-4|") << IR4 << F("}") << endl;
        Serial << F("{TIMEPLOT:IR_TX|DATA|IR1.N1:r-s4|T|") << IR1 << "}" << endl;
        Serial << F("{TIMEPLOT:IR_TX|DATA|IR2.N1:g-s4|T|") << IR2 << "}" << endl;
        Serial << F("{TIMEPLOT:IR_TX|DATA|IR3.N1:b-s4|T|") << IR3 << "}" << endl;
        Serial << F("{TIMEPLOT:IR_TX|DATA|IR4.N1:k-s4|T|") << IR4 << "}" << endl;

        break;




      case  8: //Reporting Frame: fID, nID, IR1, IR2, IR3, IR4, Dust_V, Dust_ppm, Gas_V
        data_out[0] = rx_in.substring(0, delim_pos[0]);
        data_out[1] = rx_in.substring(delim_pos[0] + 1, delim_pos[1]);
        data_out[2] = rx_in.substring(delim_pos[1] + 1, delim_pos[2]);
        data_out[3] = rx_in.substring(delim_pos[2] + 1, delim_pos[3]);
        data_out[4] = rx_in.substring(delim_pos[3] + 1, delim_pos[4]);
        data_out[5] = rx_in.substring(delim_pos[4] + 1, delim_pos[5]);
        data_out[6] = rx_in.substring(delim_pos[5] + 1, delim_pos[6]);
        data_out[7] = rx_in.substring(delim_pos[6] + 1, delim_pos[7]);
        data_out[8] = rx_in.substring(delim_pos[7] + 1);
        Serial << F("Reporting Frame - ") <<  F("FID = ") << data_out[0] << F(" , NID = ") << data_out[1] << F(" , IR_1 = ") << data_out[2]
               <<  F(" , IR_2 = ") << data_out[3] << F(" , IR_3 = ") << data_out[4] << F(" , IR_4 = ") << data_out[5] << F(" , Sensor Voltage (Dust) = ")
               << data_out[6] << F(" , Dust Concentration (ppm) = ") << data_out[7] << F(" , Sensor Voltage (Gas) = ") << data_out[8] << endl;

        nID = data_out[1].toInt();
        IR1 = data_out[2].toFloat();
        IR2 = data_out[3].toFloat();
        IR3 = data_out[4].toFloat();
        IR4 = data_out[5].toFloat();
        dust_raw = data_out[6].toFloat();
        dust_ppm = data_out[7].toFloat();
        gas_raw = data_out[8].toFloat();

        Serial << F("{TABLE:Rep_Node_1|SET|Node Address|") << data_out[1] << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|Frame ID|") << data_out[0] << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|IR-1|") << IR1 << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|IR-2|") << IR2 << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|IR-3|") << IR3 << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|IR-4|") << IR4 << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|Dust-RAW|") << dust_raw << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|Dust-ppm|") << dust_ppm << F("}") << endl;
        Serial << F("{TABLE:Rep_Node_1|SET|Gas-RAW|") << gas_raw << F("}") << endl;
        Serial << F("{TIMEPLOT:Rep_IR|DATA|IR1.N1:r-s4|T|") << IR1 << "}" << endl;
        Serial << F("{TIMEPLOT:Rep_IR|DATA|IR2.N1:g-s4|T|") << IR2 << "}" << endl;
        Serial << F("{TIMEPLOT:Rep_IR|DATA|IR3.N1:b-s4|T|") << IR3 << "}" << endl;
        Serial << F("{TIMEPLOT:Rep_IR|DATA|IR4.N1:k-s4|T|") << IR4 << "}" << endl;
        Serial << F("{TIMEPLOT:Rep_DRaw|DATA|DRaw.N1:k-s4|T|") << dust_raw << "}" << endl;
        Serial << F("{TIMEPLOT:Rep_Dppm|DATA|Dppm.N1:k-s4|T|") << dust_ppm << "}" << endl;
        Serial << F("{TIMEPLOT:Rep_GRaw|DATA|GRaw.N1:k-s4|T|") << gas_raw << "}" << endl;

        break;

      default:
        Serial << F("Incompatible FiRanger Frame") << endl;
        break;
    }


    stringComplete = false;
    delim_count = 0;
    countb = 0;
    counta = 0;
    rx_in = "";

    if (!fonaSS.stopListening()) {
      Serial << F("Error - Fona still listening") << endl;
    }
    else {
      Serial << F("Stopped Listeing to Fona Port") << endl;
      fonaSS.flush(); // DEBUG
    }
    delay(1000);

    if (!xb.listen()) {
      Serial << F("Error listening Xbee port") << endl;
    }
    else {
      Serial << F("Listening on XBee Port") << endl;
      xb.flush(); // DEBUG
    }
    Serial << F("----> End of Current Transmission") << endl;
    Serial.flush();
  }


}

void  UI_set() {

  /*
     Reporting Frame Table Creation
  */

  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|Node Address|}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|Frame ID|}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|Location|Degrees}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|IR-1|C.deg}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|IR-2|C.deg}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|IR-3|C.deg}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|IR-4|C.deg}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|Dust-RAW|Vs}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|Dust-ppm|ppm}") << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|Description|Gas-RAW|Vs}") << endl;
  Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE << F("|") << LATITUDE << F("}")  << endl;
  Serial << F("{TABLE:Rep_Node_") << nodeId << F("|SET|Location|") << LONGITUDE << F(",") << LATITUDE << F("}") <<  endl;

  /*
     Thermal Sensor Frame Table Creation
  */

  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|Node Address|}") << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|Frame ID|}") << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|Location|Degrees}") << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-Map|Active Sensor & Direction}") << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-1|C.deg}") << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-2|C.deg}") << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-3|C.deg}") << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-4|C.deg}") << endl;
  Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE << F("|") << LATITUDE << F("}")  << endl;
  Serial << F("{TABLE:IR_Node_") << nodeId << F("|SET|Location|") << LONGITUDE << F(",") << LATITUDE << F("}") <<  endl;

  /*
     Dust Sensor Frame Table Creation
  */

  Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Node Address|}") << endl;
  Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Frame ID|}") << endl;
  Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Location|Degrees}") << endl;
  Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Dust-ID|1=,2=,3=}") << endl;
  Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Dust-RAW|Vs}") << endl;
  Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Dust-ppm|ppm}") << endl;
  Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE << F("|") << LATITUDE << F("}")  << endl;
  Serial << F("{TABLE:D_Node_") << nodeId << F("|SET|Location|") << LONGITUDE << F(",") << LATITUDE << F("}") <<  endl;

  /*
     Gas Sensor Frame Table Creation
  */

  Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Node Address|}") << endl;
  Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Frame ID|}") << endl;
  Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Location|Degrees}") << endl;
  Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Gas-ID|1=,2=,3=}") << endl;
  Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Gas-RAW|Vs}") << endl;
  Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Gas-Rating|0:Normal, 1:Medium, 2: High}") << endl;
  Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE << F("|") << LATITUDE << F("}")  << endl;
  Serial << F("{TABLE:G_Node_") << nodeId << F("|SET|Location|") << LONGITUDE << F(",") << LATITUDE << F("}") <<  endl;


}


/*void  serialEvent() {

  if (Serial.available()) {

    char  inChar = (char)Serial.read();

    rx_in += inChar;

    if (inChar == '\r') {
      stringComplete = true;
    }
    if (inChar == ',') {
      delim_count++;
      delim_pos[countb] = counta;
      countb++;
    }

    counta++;
  }
  } */

