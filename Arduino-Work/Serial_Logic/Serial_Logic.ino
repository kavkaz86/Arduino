#include <SoftwareSerial.h>
//#include "MegunoLink.h"

#define  OID  'O'
#define  SID  'S'
#define  PID  'P'

//XYPlot MyPlot;
SoftwareSerial mySerial(2,3);

boolean tx_1, tx_2, tx_3, tx_toggle, valid_frame, start_ps = false; 
String tx3, tx2, tx1 = "";
char  sensorID[1];
int oxy_count, sal_count, ph_count = 0;

void setup(){

  Serial.begin(38400);
  mySerial.begin(9600); 
  tx1.reserve(20);
  tx2.reserve(20);
  tx3.reserve(20);
  /*
  MyPlot.SetTitle("Oxygen, Salinity, pH Sensors");
   MyPlot.SetXlabel("Cycle");
   MyPlot.SetYlabel("Value");
   MyPlot.SetSeriesProperties("oxy_mgL", Plot::Magenta, Plot::Solid, 2, Plot::Square);
   MyPlot.SetSeriesProperties("oxy_percentage", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
   MyPlot.SetSeriesProperties("sal_ppm", Plot::Magenta, Plot::Solid, 2, Plot::Triangle);
   MyPlot.SetSeriesProperties("sal_tdm", Plot::Blue, Plot::Dashed, 2, Plot::Circle);
   MyPlot.SetSeriesProperties("sal_uc", Plot::Green, Plot::Solid, 2, Plot::Plus);
   MyPlot.SetSeriesProperties("ph_ind", Plot::Black, Plot::Dashed, 2, Plot::Star);
   */

}
/*
void serialEvent() {
 
 char rx = (char)Serial.read(); 
 
 if(valid_frame){
 
 if(tx_3){ 
 tx3 += rx; 
 }
 else if(tx_2){ 
 tx2 +=rx; 
 }
 else if (tx_1){ 
 tx1 += rx ; 
 }  //could be replaced by else tx1 += rx; instead of elseif(tx_1) 
 }
 
 if(rx == ','){ 
 if(tx_toggle){  
 tx_2, tx_toggle = false;
 tx_3 = true; 
 }
 else{  
 tx_toggle, tx_2  = true;
 tx_1 = false;
 }
 }
 
 else if(rx ==  OID|SID|PID){
 sensorID[0] == rx; 
 valid_frame, tx_1 = true; 
 }
 else if(rx == '\r'){
 valid_frame,tx_1, tx_2, tx_3 = false;
 start_ps = true;
 }
 
 } */


void loop(){

  if(mySerial.available()){

    while(mySerial.available() > 0){

      char rx = (char)Serial.read(); 

      if(valid_frame){

        if(tx_3){ 
          tx3 += rx; 
        }
        else if(tx_2){ 
          tx2 +=rx; 
        }
        else if (tx_1){ 
          tx1 += rx ; 
        }  //could be replaced by else tx1 += rx; instead of elseif(tx_1) 
      }

      if(rx == ','){ 
        if(tx_toggle){  
          tx_2, tx_toggle = false;
          tx_3 = true; 
        }
        else{  
          tx_toggle, tx_2  = true;
          tx_1 = false;
        }
      }

      else if(rx ==  OID|SID|PID){
        sensorID[0] == rx; 
        valid_frame, tx_1 = true; 
      }
      else if(rx == '\r'){
        valid_frame,tx_1, tx_2, tx_3 = false;
        start_ps = true;
      }
    }
  }




  if(start_ps){
    /*
    switch(sensorID[0]){
     
     case  OID:
     
     MyPlot.SendData("oxy_percentage", oxy_count,  tx1); 
     delay(500);
     MyPlot.SendData("oxy_mgL", oxy_count,  tx2); 
     delay(500);
     oxy_count++;
     break;
     
     case  SID:
     
     MyPlot.SendData("sal_ppm", sal_count,  tx1); 
     delay(500);
     MyPlot.SendData("sal_tdm", sal_count,  tx2); 
     delay(500);
     MyPlot.SendData("sal_uc", sal_count,  tx3); 
     delay(500);
     sal_count++;
     break;
     
     case  PID:
     
     MyPlot.SendData("ph_ind", ph_count,  tx1); 
     delay(500);  
     ph_count++;    
     break;
     
     default:  
     break;
     
     }
     */

    tx1,tx2,tx3 = "";
    start_ps = false;
    sensorID[0] = '\0';
  }  

}








