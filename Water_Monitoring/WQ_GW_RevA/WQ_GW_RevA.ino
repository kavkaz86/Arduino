#include <SoftwareSerial.h>
#include "MegunoLink.h"

#define  LONG       "-31.752426"
#define  LAT        "115.770962"
#define  TEMP       'A'
#define  UV         'C'
#define  PAR        'D'
#define  TURBIDTY   'H'
#define  OXYGEN     'O'
#define  SALINITY   'S'
#define  PH         'P'
#define  cpy_start  ':'
#define  DELIMITER  ','
#define  RX2_START  1
#define  RX3_START  2

XYPlot MyPlot;
Map MyMap;
Table table;
SoftwareSerial mySerial(2,3);

boolean  tx1_cpy = false;
boolean  tx2_cpy = false;
boolean  tx3_cpy = false;
boolean  rx_complete = false; 

char  sensor_id[1];
uint8_t  del_count = 0; 
String  rx_1 = "";
String  rx_2 = "";
String  rx_3 = ""; 
int  oxy_count = 0;
int  sal_count = 0;
int  ph_count = 0;
int  temp_count = 0;
int  uv_count = 0;
int  par_count = 0; 
int  tur_count = 0;

void setup(){

  Serial.begin(115200); 
  mySerial.begin(9600);
  rx_1.reserve(20);
  rx_2.reserve(20);
  rx_3.reserve(20);
  
  MyPlot.SetTitle("Water Quality GW Rev-A");
  MyPlot.SetXlabel("Cycle");
  MyPlot.SetYlabel("Value");
  MyPlot.SetSeriesProperties("temp_c", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
  MyPlot.SetSeriesProperties("turbidity_mv", Plot::Blue, Plot::Dashed, 2, Plot::Circle);
  MyPlot.SetSeriesProperties("uv_ppf", Plot::Green, Plot::Solid, 2, Plot::Plus);
  MyPlot.SetSeriesProperties("par_ppf", Plot::Black, Plot::Dashed, 2, Plot::Star);
  MyPlot.SetSeriesProperties("oxy_mgL", Plot::Magenta, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("oxy_percentage", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
  MyPlot.SetSeriesProperties("sal_ppm", Plot::Magenta, Plot::Solid, 2, Plot::Triangle);
  MyPlot.SetSeriesProperties("sal_tdm", Plot::Blue, Plot::Dashed, 2, Plot::Circle);
  MyPlot.SetSeriesProperties("sal_uc", Plot::Green, Plot::Solid, 2, Plot::Plus);
  MyPlot.SetSeriesProperties("ph_ind", Plot::Black, Plot::Dashed, 2, Plot::Star);
  MyMap.SendData("Location", LONG, LAT);
}

void loop(){

  if(mySerial.available()){

    char inchar = (char)mySerial.read(); 

    if( (inchar == OXYGEN) || (inchar == SALINITY) || (inchar == PH) || 
      (inchar == TEMP) || (inchar == UV) || (inchar == PAR) || (inchar == TURBIDTY) )
    {
      sensor_id[0] = inchar; 
    }
    if( inchar == '\r'){
      tx1_cpy = false; 
      tx2_cpy = false; 
      tx3_cpy = false; 
      rx_complete = true;
      del_count = 0;  
    }

    if( inchar == cpy_start ){
      tx1_cpy = true; 
    }

    if( inchar == DELIMITER){
      del_count++; 
      if(del_count == RX2_START)
      {
        tx1_cpy = false;
        tx2_cpy = true;
      }
      else if(del_count == RX3_START){
        tx2_cpy = false;
        tx3_cpy = true;
      }
    }

    if( (tx1_cpy) && (inchar!=cpy_start) ){  
      rx_1 += inchar; 
    }

    if( (tx2_cpy) && (inchar!=DELIMITER) ){
      rx_2 += inchar; 
    }

    if( (tx3_cpy) && (inchar!=DELIMITER) ){
      rx_3 += inchar; 
    }

  }



  if(rx_complete){

    switch(sensor_id[0]){

    case  OXYGEN:

      MyPlot.SendData("oxy_percentage", oxy_count,  rx_1); //delay(500);
      MyPlot.SendData("oxy_mgL", oxy_count,  rx_2); ///delay(500);
      oxy_count++;
      break;

    case  SALINITY:

      MyPlot.SendData("sal_ppm", sal_count,  rx_1); //delay(500);
      MyPlot.SendData("sal_tdm", sal_count,  rx_2); //delay(500);
      MyPlot.SendData("sal_uc", sal_count,  rx_3); //delay(500);
      sal_count++;
      break;

    case  PH:

      MyPlot.SendData("ph_ind", ph_count,  rx_1); //delay(500);  
      ph_count++;    
      break;
     
     
    case  TEMP:
      
      MyPlot.SendData("temp_c", temp_count,  rx_1);  
      temp_count++;
      break; 
    
    case  PAR:
      
      MyPlot.SendData("par_ppf", par_count,  rx_1);  
      par_count++;
      break;
    
    case  UV:
      
      MyPlot.SendData("uv_ppf", uv_count,  rx_1);  
      uv_count++;
      break;
      
    case  TURBIDTY:
      
      MyPlot.SendData("turbidity_mv", tur_count,  rx_1);  
      tur_count++;
      break;       

    default: 
      break;
    }

    rx_1 = "";
    rx_2 = "";  
    rx_3 = "";      
    rx_complete = false; 
    sensor_id[0] = '\0'; 
  }

}


/*
void serialEvent(){
 
 char inchar = (char)Serial.read(); 
 
 if( (inchar == OXYGEN) || (inchar == SALINITY) || (inchar == PH) )
 {
 sensor_id[0] = inchar; 
 }
 if( inchar == '\r'){
 tx1_cpy = false; 
 tx2_cpy = false; 
 tx3_cpy = false; 
 rx_complete = true;
 del_count = 0;  
 }
 
 if( inchar == cpy_start ){
 tx1_cpy = true; 
 }
 
 if( inchar == DELIMITER){
 del_count++; 
 if(del_count == RX2_START)
 {
 tx1_cpy = false;
 tx2_cpy = true;
 }
 else if(del_count == RX3_START){
 tx2_cpy = false;
 tx3_cpy = true;
 }
 }
 
 if( (tx1_cpy) && (inchar!=cpy_start) ){  
 rx_1 += inchar; 
 }
 
 if( (tx2_cpy) && (inchar!=DELIMITER) ){
 rx_2 += inchar; 
 }
 
 if( (tx3_cpy) && (inchar!=DELIMITER) ){
 rx_3 += inchar; 
 }
 }
 */





