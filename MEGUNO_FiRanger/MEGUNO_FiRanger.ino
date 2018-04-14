#include  "Streaming.h"
#include  "SoftwareSerial.h"
#include "MegunoLink.h"

#define RXD 2
#define TXD 3


int delim_count = 0;
int counta = 0;
bool  stringComplete = false;
int delim_pos[20];
String  rx_in = "";
int countb = 0;
String  data_out[20];

String  LONGITUDE[] = {"-32.174351", "-32.180599"};
String  LATITUDE[] = {"116.050567", "116.069107"};

SoftwareSerial  xb(RXD, TXD);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  xb.begin(9600); delay(50);

  rx_in.reserve(200);

  UI_set();

}

void  UI_set() {

  /*
     Reporting Frame Table Creation
  */
  for (int a = 1; a < 3 ; a++) {

    String nodeId = String(a, DEC);
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
    Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE[a - 1] << F("|") << LATITUDE[a - 1] << F("}")  << endl;
    Serial << F("{TABLE:Rep_Node_") << nodeId << F("|SET|Location|") << LONGITUDE[a - 1] << F(",") << LATITUDE[a - 1] << F("}") <<  endl;
    nodeId = "";
  }

  /*
     Thermal Sensor Frame Table Creation
  */
  for (int a = 1; a < 3 ; a++) {

    String nodeId = String(a, DEC);
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|Node Address|}") << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|Frame ID|}") << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|Location|Degrees}") << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-Map|Active Sensor & Direction}") << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-1|C.deg}") << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-2|C.deg}") << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-3|C.deg}") << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|Description|IR-4|C.deg}") << endl;
    Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE[a - 1] << F("|") << LATITUDE[a - 1] << F("}")  << endl;
    Serial << F("{TABLE:IR_Node_") << nodeId << F("|SET|Location|") << LONGITUDE[a - 1] << F(",") << LATITUDE[a - 1] << F("}") <<  endl;
    nodeId = "";
  }

  /*
     Dust Sensor Frame Table Creation
  */
  for (int a = 1; a < 3 ; a++) {

    String nodeId = String(a, DEC);
    Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Node Address|}") << endl;
    Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Frame ID|}") << endl;
    Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Location|Degrees}") << endl;
    Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Dust-ID|1=,2=,3=}") << endl;
    Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Dust-RAW|Vs}") << endl;
    Serial << F("{TABLE:D_Node_") << nodeId << F("|Description|Dust-ppm|ppm}") << endl;
    Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE[a - 1] << F("|") << LATITUDE[a - 1] << F("}")  << endl;
    Serial << F("{TABLE:D_Node_") << nodeId << F("|SET|Location|") << LONGITUDE[a - 1] << F(",") << LATITUDE[a - 1] << F("}") <<  endl;
    nodeId = "";
  }

  /*
     Gas Sensor Frame Table Creation
  */
  for (int a = 1; a < 3 ; a++) {

    String nodeId = String(a, DEC);
    Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Node Address|}") << endl;
    Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Frame ID|}") << endl;
    Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Location|Degrees}") << endl;
    Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Gas-ID|1=,2=,3=}") << endl;
    Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Gas-RAW|Vs}") << endl;
    Serial << F("{TABLE:G_Node_") << nodeId << F("|Description|Gas-Rating|0:Normal, 1:Medium, 2: High}") << endl;
    Serial << F("{MAP|SET|NODE_") << nodeId << F("|") << LONGITUDE[a - 1] << F("|") << LATITUDE[a - 1] << F("}")  << endl;
    Serial << F("{TABLE:G_Node_") << nodeId << F("|SET|Location|") << LONGITUDE[a - 1] << F(",") << LATITUDE[a - 1] << F("}") <<  endl;
    nodeId = "";
  }

}

void loop() {

  /* if (xb.available()) {

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
  } */


  /*
     Reporting Frame: fID, nID, IR1, IR2, IR3, IR4, Dust_V, Dust_ppm, Gas_V  - 8 delims
     Dust Frame:  fID, nID, Dust_ID, Dust_V, Dust_ppm   - 4 delims
     Primary IR Detection:  fID, nID, IR_MAP, IR_1, IR_2, IR_3, IR_4  - 6 delims
     Gas Frame: fID, nID, GasID, Gas_V - 3 delims
  */
  
  if (stringComplete) {

    Serial << "InputString = " << rx_in << " | Number of Delim = " << delim_count << " | Size of Input = " << rx_in.length() << endl;

    for (int i = 0; i < delim_count; i++) {
      Serial << "Delimiter Position " << i << " = " <<  delim_pos[i] << endl;
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
        
        switch (nID) {
          case  5:
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

          case  7:
            Serial << F("{TABLE:G_Node_2|SET|Node Address|") << data_out[1] << F("}") << endl;
            Serial << F("{TABLE:G_Node_2|SET|Frame ID|") << data_out[0] << F("}") << endl;
            Serial << F("{TABLE:G_Node_2|SET|Gas-ID|") << data_out[2] << F("}") << endl;
            Serial << F("{TABLE:G_Node_2|SET|Gas-RAW|") << gas_raw << F("}") << endl;
            if (gas_raw > 3.5) {
              Serial << F("{TABLE:G_Node_2|SET|Gas-Rating|2}") << endl;
            }
            else if (gas_raw > 2.0 && gas_raw < 3.5) {
              Serial << F("{TABLE:G_Node_2|SET|Gas-Rating|1}")  << endl;
            }
            else
              Serial << F("{TABLE:G_Node_2|SET|Gas-Rating|0}") << endl;
              
            Serial << F("{TIMEPLOT:G_TX|DATA|G.N2:k-s4|T|") << gas_raw << "}" << endl;
            break;

          default:
            Serial << "Unknown Node Address" << endl;
            break;
        }

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

        switch (nID) {
          case  5:
            Serial << F("{TABLE:D_Node_1|SET|Node Address|") << data_out[1] << F("}") << endl;
            Serial << F("{TABLE:D_Node_1|SET|Frame ID|") << data_out[0] << F("}") << endl;
            Serial << F("{TABLE:D_Node_1|SET|Dust-ID|") << data_out[2] << F("}") << endl;
            Serial << F("{TABLE:D_Node_1|SET|Dust-RAW|") << dust_raw << F("}") << endl;
            Serial << F("{TABLE:D_Node_1|SET|Dust-ppm|") << dust_ppm << F("}") << endl;
            Serial << F("{TIMEPLOT:DRaw_TX|DATA|DRaw.N1:r-s4|T|") << dust_raw << "}" << endl;
            Serial << F("{TIMEPLOT:Dppm_TX|DATA|Dppm.N2:r-s4|T|") << gas_raw << "}" << endl;
            break;

          case  7:
            Serial << F("{TABLE:D_Node_2|SET|Node Address|") << data_out[1] << F("}") << endl;
            Serial << F("{TABLE:D_Node_2|SET|Frame ID|") << data_out[0] << F("}") << endl;
            Serial << F("{TABLE:D_Node_2|SET|Dust-ID|") << data_out[2] << F("}") << endl;
            Serial << F("{TABLE:D_Node_2|SET|Dust-RAW|") << dust_raw << F("}") << endl;
            Serial << F("{TABLE:D_Node_2|SET|Dust-ppm|") << dust_ppm << F("}") << endl;
            Serial << F("{TIMEPLOT:DRaw_TX|DATA|DRaw.N2:k-s4|T|") << dust_raw << "}" << endl;
            Serial << F("{TIMEPLOT:Dppm_TX|DATA|Dppm.N2:k-s4|T|") << gas_raw << "}" << endl;
            break;

          default:
            Serial << "Unknown Node Address" << endl;
            break;
        }



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

        switch (nID) {
          case  5:
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

          case  7:
            Serial << F("{TABLE:IR_Node_2|SET|Node Address|") << data_out[1] << F("}") << endl;
            Serial << F("{TABLE:IR_Node_2|SET|Frame ID|") << data_out[0] << F("}") << endl;
            Serial << F("{TABLE:IR_Node_2|SET|IR-Map|") << data_out[2] << F("}") << endl;
            Serial << F("{TABLE:IR_Node_2|SET|IR-1|") << IR1 << F("}") << endl;
            Serial << F("{TABLE:IR_Node_2|SET|IR-2|") << IR2 << F("}") << endl;
            Serial << F("{TABLE:IR_Node_2|SET|IR-3|") << IR3 << F("}") << endl;
            Serial << F("{TABLE:IR_Node_2|SET|IR-4|") << IR4 << F("}") << endl;
            Serial << F("{TIMEPLOT:IR_TX|DATA|IR1.N2:r;s4|T|") << IR1 << "}" << endl;
            Serial << F("{TIMEPLOT:IR_TX|DATA|IR2.N2:g;s4|T|") << IR2 << "}" << endl;
            Serial << F("{TIMEPLOT:IR_TX|DATA|IR3.N2:b;s4|T|") << IR3 << "}" << endl;
            Serial << F("{TIMEPLOT:IR_TX|DATA|IR4.N2:k;s4|T|") << IR4 << "}" << endl;

            break;

          default:
            Serial << "Unknown Node Address" << endl;
            break;
        }

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
        
        switch (nID) {
          case  5:
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

          case  7:
            Serial << F("{TABLE:Rep_Node_2|SET|Node Address|") << data_out[1] << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|Frame ID|") << data_out[0] << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|IR-1|") << IR1 << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|IR-2|") << IR2 << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|IR-3|") << IR3 << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|IR-4|") << IR4 << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|Dust-RAW|") << dust_raw << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|Dust-ppm|") << dust_ppm << F("}") << endl;
            Serial << F("{TABLE:Rep_Node_2|SET|Gas-RAW|") << gas_raw << F("}") << endl;
            Serial << F("{TIMEPLOT:Rep_IR|DATA|IR1.N2:r;s4|T|") << IR1 << "}" << endl;
            Serial << F("{TIMEPLOT:Rep_IR|DATA|IR2.N2:g;s4|T|") << IR2 << "}" << endl;
            Serial << F("{TIMEPLOT:Rep_IR|DATA|IR3.N2:b;s4|T|") << IR3 << "}" << endl;
            Serial << F("{TIMEPLOT:Rep_IR|DATA|IR4.N2:k;s4|T|") << IR4 << "}" << endl;
            Serial << F("{TIMEPLOT:Rep_DRaw|DATA|DRaw.N2:k;s4|T|") << dust_raw << "}" << endl;
            Serial << F("{TIMEPLOT:Rep_Dppm|DATA|Dppm.N2:k;s4|T|") << dust_ppm << "}" << endl;
            Serial << F("{TIMEPLOT:Rep_GRaw|DATA|GRaw.N2:k;s4|T|") << gas_raw << "}" << endl;
            break;

          default:
            Serial << "Unknown Node Address" << endl;
            break;
        }

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
  }


}

 void  serialEvent() {

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
} 

