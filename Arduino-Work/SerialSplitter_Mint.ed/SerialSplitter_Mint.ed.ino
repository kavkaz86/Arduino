#include  "Streaming.h"
#include  "SoftwareSerial.h"

SoftwareSerial  rf(2,3); 

#define repF  "1"
#define irF   "2"
#define gasF  "3"
#define diagF "4"

#define CO2_ID "1"
#define CH4_ID "2"
#define LPG_ID "3"
#define CO_ID  "4"
#define AD_ID  "5"
#define ALL_SENS  "6"

#define  rep_handle   1
#define IR_handle     2
#define gas_handle    3
#define diag_handle   4

#define  WTH_addr  "10"


String f_id = "";         // a string to hold incoming data
String n_addr = "";
String IR_flag = "";
String rep_flag = "";
String IR_1 = "";
String IR_2 = "";
String IR_3 = "";
String IR_4 = "";
String Dust_V = "";
String Dust_ppm = "";
String Gas_V = "";
String Gas_ppm = "";
String winddir = "";
String windspeedmph = "";
String windgustmph = "";
String windspdmph_avg2m = "";
String winddir_avg2m = "";
String windgustmph_10m = "";
String windgustdir_10m = "";
String humidity = "";
String tempf = "";
String rainin = "";
String dailyrainin = "";
String pressure = "";
String batt_lvl = "";


String    input1 = "";
String    input2 = "";
String    input3 = "";
String    input4 = "";
String    input5 = "";
String    input6 = "";
String    input7 = "";
String    input8 = "";
String    input9 = "";
String    input10 = "";
String    input11 = "";
String    input12 = "";
String    input13 = "";
String    input14 = "";
String    input15 = "";


boolean stringComplete = false;// whether the string is complete
int contA = 0;
boolean data_valid = true;
int data_flag = 0;
int handler_flag = 0;

void setup() {
  // initialize serial:

  Serial.begin(38400);
  rf.begin(38400);
  /*
    f_id
    n_addr
    IR_flag
    IR_1
    IR_2
    IR_3
    IR_4
    Dust_V
    Dust_ppm
    Gas_V
    winddir
    windspeedmph
    windgustmph
    windspdmph_avg2m
    winddir_avg2m
    windgustmph_10m
    windgustdir_10m
    humidity
    tempf
    rainin
    dailyrainin
    pressure
    batt_lvl
  */

  pgm_vars();

}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {

    input_trim();

    if (input1 == repF) handler_flag = rep_handle;
    else if (input1 == irF) handler_flag = IR_handle;
    else if (input1 == gasF) handler_flag = gas_handle;
    else if (input1 == diagF) handler_flag = diag_handle;

    switch (handler_flag) {

      case  rep_handle:
        repF_handler();
        break;
      case  IR_handle:
        irF_handler();
        break;
      case  gas_handle:
        gasF_handler();
        break;
      case  diag_handle:
        //diagF_handler();
        break;
      default: break;
    }

    str_trim();

    display_data();


    // clear the string:
    str_clear();
    stringComplete = false;
    data_valid = false;
    contA = 0;
    data_flag = 0;
  }
}


void  display_data() {


  
  /*switch(dsp_flag){

    case  rep_handle:
        
  }*/

  if (f_id.length() > 0 ) Serial << "Frame-ID: " << f_id << endl;
  if (n_addr.length() > 0 ) Serial << "Node-Address: " << n_addr << endl;
  if (IR_flag.length() > 0 ) Serial << "IR-Flag: " << IR_flag << endl;
  if (rep_flag.length() > 0 ) Serial << "Reporting-Flag: " << rep_flag << endl;
  if (IR_1.length() > 0 ) Serial << "IR 1: " << IR_1 << endl;
  if (IR_2.length() > 0 ) Serial << "IR 2: " << IR_2 << endl;
  if (IR_3.length() > 0 ) Serial << "IR 3: " << IR_3 << endl;
  if (IR_4.length() > 0 ) Serial << "IR 4: " << IR_4 << endl;
  if (Dust_V.length() > 0 ) Serial << "Dust Voltage: " << Dust_V << endl;
  if (Dust_ppm.length() > 0 ) Serial << "Dust ppm: " << Dust_ppm << endl;
  if (Gas_V.length() > 0 ) Serial << "Gas Voltage: " << Gas_V << endl;
  if (winddir.length() > 0 ) Serial << "Wind Direction: " << winddir << endl;
  if (windspeedmph.length() > 0 ) Serial << "Wind Speed (mph): " << windspeedmph << endl;
  if (windgustmph.length() > 0 ) Serial << "Wing Gust (mph): " << windgustmph << endl;
  if (windspdmph_avg2m.length() > 0 ) Serial << "Average Wind Speed (2 min): " << windspdmph_avg2m << endl;
  if (winddir_avg2m.length() > 0 ) Serial << "Average Wind Direction (2 min): " << winddir_avg2m << endl;
  if (windgustmph_10m.length() > 0 ) Serial << "Average Wind Gust (10 min): " << windgustmph_10m << endl;
  if (windgustdir_10m.length() > 0 ) Serial << "Average Wind Direction (10 min): " << windgustdir_10m << endl;
  if (humidity.length() > 0 ) Serial << "Humidity: " << humidity << endl;
  if (tempf.length() > 0 ) Serial << "Temperature (F): " << tempf << endl;
  if (rainin.length() > 0 ) Serial << "Rainfall: " << rainin << endl;
  if (dailyrainin.length() > 0 ) Serial << "Daily rainfall: " << dailyrainin << endl;
  if (pressure.length() > 0 ) Serial << "Pressure: " << pressure << endl;
  if (batt_lvl.length() > 0 ) Serial << "Battery (Voltage): " << batt_lvl << endl;

}

void  input_trim() {

  /*f_id.trim();
  n_addr.trim();
  IR_1.trim();
  IR_2.trim();
  IR_3.trim();
  IR_4.trim();
  Dust_V.trim();
  Dust_ppm.trim();
  Gas_V.trim();
  winddir.trim();
  windspeedmph.trim();
  windgustmph.trim();
  windspdmph_avg2m.trim();
  winddir_avg2m.trim();
  windgustmph_10m.trim();
  windgustdir_10m.trim();
  humidity.trim();
  tempf.trim();
  rainin.trim();
  dailyrainin.trim();
  pressure.trim();
  batt_lvl.trim(); */

  input1.trim();
  input2.trim();
  input3.trim();
  input4.trim();
  input5.trim();
  input6.trim();
  input7.trim();
  input8.trim();
  input9.trim();
  input10.trim();
  input11.trim();
  input12.trim();
  input13.trim();
  input14.trim();
  input15.trim();

}

void  str_trim() {
  f_id.trim();
  n_addr.trim();
  IR_flag.trim();
  rep_flag.trim();
  IR_1.trim();
  IR_2.trim();
  IR_3.trim();
  IR_4.trim();
  Dust_V.trim();
  Dust_ppm.trim();
  Gas_V.trim();
  winddir.trim();
  windspeedmph.trim();
  windgustmph.trim();
  windspdmph_avg2m.trim();
  winddir_avg2m.trim();
  windgustmph_10m.trim();
  windgustdir_10m.trim();
  humidity.trim();
  tempf.trim();
  rainin.trim();
  dailyrainin.trim();
  pressure.trim();
  batt_lvl.trim();
}


void irF_handler() {

  f_id = input1;
  n_addr = input2;
  IR_flag = input3;
  IR_1 = input4;
  IR_2 = input5;
  IR_3 = input6;
  IR_4 = input7;

}

void gasF_handler() {

 //gasF, NODE_ID[0], LPG_ID, GAS_vals
 //gasF, NODE_ID[0], AD_ID, DUST_vals, 2
  f_id = input1;
  n_addr = input2;
  rep_flag = input3; 
  if(rep_flag == AD_ID){
    Dust_V = input4;
    Dust_ppm = input5; 
  }
  else if(rep_flag == CO2_ID){
    Gas_V = input4;
    Gas_ppm = input5;
  }
  else{
    Gas_V = input4;
  }
 
}

void repF_handler() {


  if (input2 == WTH_addr) {

    f_id = input1;
    n_addr = input2;
    winddir = input3;
    windspeedmph = input4;
    windgustmph = input5;
    windspdmph_avg2m = input6;
    winddir_avg2m = input7;
    windgustmph_10m = input8;
    windgustdir_10m = input9;
    humidity = input10;
    tempf = input11;
    rainin = input12;
    dailyrainin = input13;
    pressure = input14;
    batt_lvl = input15;

  }

  else {

    f_id = input1;
    n_addr = input2;
    IR_1 = input3;
    IR_2 = input4;
    IR_3 = input5;
    IR_4 = input6;
    Dust_V = input7;
    Dust_ppm = input8;
    Gas_V = input9;

  }

}


void serialEvent() {
  while (rf.available()) {

    char inChar = (char)rf.read();
    delay(10);

    if (inChar == ',') {

      contA++;
    }

    if (inChar != ',') {

      switch (contA) {
        case  0:
          input1 += inChar;
          break;

        case  1:
          input2 += inChar;
          break;

        case  2:
          input3 += inChar;
          break;

        case  3:
          input4 += inChar;
          break;

        case  4:
          input5 += inChar;
          break;

        case  5:
          input6 += inChar;
          break;

        case  6:
          input7 += inChar;
          break;

        case  7:
          input8 += inChar;
          break;

        case  8:
          input9 += inChar;
          break;

        case  9:
          input10 += inChar;
          break;

        case  10:
          input11 += inChar;
          break;

        case  11:
          input12 += inChar;
          break;

        case  12:
          input13 += inChar;
          break;

        case  13:
          input14 += inChar;
          break;

        case  14:
          input15 += inChar;
          break;

        default: break;
      }

    }

    if (inChar == '\r' || '\n') {

      stringComplete = true;
    }

  }

}

void  pgm_vars() {

  f_id.reserve(10);
  n_addr.reserve(10);
  IR_flag.reserve(10);
  rep_flag.reserve(10);
  IR_1.reserve(10);
  IR_2.reserve(10);
  IR_3.reserve(10);
  IR_4.reserve(10);
  Dust_V.reserve(10);
  Dust_ppm.reserve(10);
  Gas_V.reserve(10);
  Gas_ppm.reserve(10);
  winddir.reserve(10);
  windspeedmph.reserve(10);
  windgustmph.reserve(10);
  windspdmph_avg2m.reserve(10);
  winddir_avg2m.reserve(10);
  windgustmph_10m.reserve(10);
  windgustdir_10m.reserve(10);
  humidity.reserve(10);
  tempf.reserve(10);
  rainin.reserve(10);
  dailyrainin.reserve(10);
  pressure.reserve(10);
  batt_lvl.reserve(10);

  input1.reserve(10);
  input2.reserve(10);
  input3.reserve(10);
  input4.reserve(10);
  input5.reserve(10);
  input6.reserve(10);
  input7.reserve(10);
  input8.reserve(10);
  input9.reserve(10);
  input10.reserve(10);
  input11.reserve(10);
  input12.reserve(10);
  input13.reserve(10);
  input14.reserve(10);
  input15.reserve(10);
}

void  str_clear() {

  input1 = "";
  input2 = "";
  input3 = "";
  input4 = "";
  input5 = "";
  input6 = "";
  input7 = "";
  input8 = "";
  input9 = "";
  input10 = "";
  input11 = "";
  input12 = "";
  input13 = "";
  input14 = "";
  input15 = "";
  f_id = "";
  n_addr = "";
  IR_flag = "";
  rep_flag = "";
  IR_1 = "";
  IR_2 = "";
  IR_3 = "";
  IR_4 = "";
  Dust_V = "";
  Dust_ppm = "";
  Gas_V = "";
  Gas_ppm = "";
  winddir = "";
  windspeedmph = "";
  windgustmph = "";
  windspdmph_avg2m = "";
  winddir_avg2m = "";
  windgustmph_10m = "";
  windgustdir_10m = "";
  humidity = "";
  tempf = "";
  rainin = "";
  dailyrainin = "";
  pressure = "";
  batt_lvl = "";
}



