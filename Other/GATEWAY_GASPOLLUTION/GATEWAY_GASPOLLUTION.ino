/*
Colors = { Red, Green, Blue, Yellow, Black, Magenta, Cyan, White }
LineStyle = { Solid, Dashed, Dotted, DashDot, DashDotDot, NoLine }
MarkerStyle = {Square, Diamond, Triangle, Circle, Cross, Plus, Star, DownwardTriangle, NoMarker }
*/

#include "MegunoLink.h"
#include <SoftwareSerial.h>


/*************INCOMING RX_FRAME SENSOR_IDs ********* 

              Temp_ID          'A'
              Oxygen_ID        'B'
              NDioxide_ID      'C'
              Methane_ID       'D'
              Ammonia_ID       'E'
              AirPoll_ID       'F'
              CarbDioxide_ID   'G'
              
****************************************************
*/
#define TEMP              'A'
#define OXYGEN            'B'
#define NITROGEN_DIOXIDE  'C' 
#define METHANE           'D'
#define AMMONIA           'E'
#define AIR_POLLUTION     'F'
#define CARBON_DIOXIDE    'G'
#define DELIMITER         ':'

XYPlot MyPlot;
SoftwareSerial mySerial(2,3);

boolean serial_tx_cpy = false; 
boolean tx_complete = false; 
boolean ID_flag = false;
String sensorvalue = ""; 
char sensor_ID[1]; 
int  temp_counter = 0;
int  oxygen_counter = 0;
int  ndioxide_counter = 0;
int  methane_counter =0;
int  ammonia_counter = 0;
int  airpoll_counter = 0;
int  carbdioxide_counter = 0; 

void setup(){

  Serial.begin(9600);
  mySerial.begin(9600);

  delay(5000); 
  sensorvalue.reserve(30);  
  
  MyPlot.SetTitle("Air Pollution Measurements");
  MyPlot.SetXlabel("Cycle");
  MyPlot.SetYlabel("Gas (V)");
  MyPlot.SetSeriesProperties("T-C", Plot::Magenta, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("O2", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
  MyPlot.SetSeriesProperties("NO2", Plot::Magenta, Plot::Solid, 2, Plot::Triangle);
  MyPlot.SetSeriesProperties("CH4", Plot::Blue, Plot::Dashed, 2, Plot::Circle);
  MyPlot.SetSeriesProperties("NH3", Plot::Green, Plot::Solid, 2, Plot::Plus);
  MyPlot.SetSeriesProperties("AP", Plot::Black, Plot::Dashed, 2, Plot::Star);
  MyPlot.SetSeriesProperties("CO2", Plot::Black, Plot::Solid, 2, Plot::DownwardTriangle); 

}



void loop(){


  if(mySerial.available()){

    char inchar = (char)mySerial.read();

    if( (inchar == TEMP) || (inchar == OXYGEN) || (inchar == NITROGEN_DIOXIDE) ||   
      (inchar == METHANE) || (inchar == AMMONIA) || (inchar == AIR_POLLUTION) || 
      (inchar == CARBON_DIOXIDE) )  
    {
      sensor_ID[0] = inchar;
      ID_flag = true;  
    }

    if(serial_tx_cpy && ID_flag){
      sensorvalue += inchar;
      if(inchar == '\r' && ID_flag){
        tx_complete = true;
        serial_tx_cpy = false;
        ID_flag = false;
      }
    }

    if(inchar == DELIMITER && ID_flag){
      serial_tx_cpy = true;
    }
  }

/******************* GAS IDs: **********************
         TEMP              'A'
         OXYGEN            'B'
         NITROGEN_DIOXIDE  'C' 
         METHANE           'D'
         AMMONIA           'E'
         AIR_POLLUTION     'F'
         CARBON_DIOXIDE    'G'
         DELIMITER         ':'
 ******************* counters **********************         
         int  temp_counter = 0;
         int  oxygen_counter = 0;
         int  ndioxide_counter = 0;
         int  methane_counter =0;
         int  ammonia_counter = 0;
         int  airpoll_counter = 0;
         int  carbdioxide_counter = 0; 
****************************************************
*/
  if (tx_complete){ 
    switch(sensor_ID[0]){
    case  TEMP:           
      MyPlot.SendData("T-C", temp_counter,  sensorvalue);
      temp_counter++;    
      break;

    case  OXYGEN:     
      MyPlot.SendData("O2", oxygen_counter,  sensorvalue); 
      oxygen_counter++;   
      break; 

    case  NITROGEN_DIOXIDE:      
      MyPlot.SendData("NO2", ndioxide_counter,  sensorvalue);
      ndioxide_counter++;      
      break; 

    case  METHANE:
      methane_counter++;
      MyPlot.SendData("CH4", methane_counter,  sensorvalue);      
      break;
   
    case  AMMONIA:      
      MyPlot.SendData("NH3", ammonia_counter,  sensorvalue);
      ammonia_counter++;      
      break;
    
    case  AIR_POLLUTION:      
      MyPlot.SendData("AP", airpoll_counter,  sensorvalue);
      airpoll_counter++;      
      break;
    
    case  CARBON_DIOXIDE:     
      MyPlot.SendData("CO2", carbdioxide_counter,  sensorvalue);
      carbdioxide_counter++;      
      break;   

    default:
      break;

    }
    sensorvalue = "";                                                       
    tx_complete = false;
    sensor_ID[0] = NULL ;
  }
}


//******************************//
//            EXTRA CODE        //
//******************************//

/*if (tx_complete){
 
 switch(sensor_ID[0]){
 log_time = millis();
 case  TEMP_1:       
 MyPlot.SendData("Temperature_1 (C)", log_time,  sensorvalue);
 //Serial.print("TEMPERATURE_1:"); 
 //Serial.println(sensorvalue); 
 break;
 
 case  TEMP_2:
 //Serial.print("TEMPERATURE_2:"); 
 // Serial.println(sensorvalue); 
 break; //DEBUG
 
 case  UV:
 // Serial.print("UV:"); 
 //Serial.println(sensorvalue); 
 break;  //DEBUG
 
 case  PAR:
 //Serial.print("PAR:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG
 
 case  OXYGEN:
 MyPlot.SendData("Oxygen (MG/L)", log_time,  sensorvalue);
 //Serial.print("OXYGEN:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG
 
 case  SALINITY:
 // Serial.print("SALINITY:"); 
 // Serial.println(sensorvalue); 
 break; //DEBUG
 
 case  PH:
 //Serial.print("PH:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG
 
 case  TURBIDTY:
 //Serial.print("TURBIDTY:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG
 
 default:break;
 
 }  */


/*
void serialEvent(){
 
 char inchar = (char)Serial1.read();
 
 if( (inchar == TEMP_1) || (inchar == TEMP_2) || (inchar == UV) ||   
 (inchar == PAR) || (inchar == OXYGEN) || (inchar == SALINITY) || 
 (inchar == PH) || (inchar == TURBIDTY) )
 {
 sensor_ID[0] = inchar;
 ID_flag = true;  
 }
 
 if(serial_tx_cpy && ID_flag){
 sensorvalue += inchar;
 if(inchar == '\r' && ID_flag){
 tx_complete = true;
 serial_tx_cpy = false;
 ID_flag = false;
 }
 
 }
 
 if(inchar == DELIMITER && ID_flag){
 serial_tx_cpy = true;
 }
 
 }
 
/*case  TEMP_2:
 MyPlot.SendData("Temperature_1 (C)", log_time,  sensorvalue);
 //Serial.print("TEMPERATURE_2:"); 
 // Serial.println(sensorvalue); 
 break; //DEBUG 
 
 case  UV:
 MyPlot.SendData("UV", log_time,  sensorvalue);
 // Serial.print("UV:"); 
 //Serial.println(sensorvalue); 
 break;  //DEBUG
 
 case  PAR:
 MyPlot.SendData("PAR", log_time,  sensorvalue);
 //Serial.print("PAR:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG
 
 case  OXYGEN:
 MyPlot.SendData("Oxygen (MG/L)", log_time,  sensorvalue);
 //Serial.print("OXYGEN:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG
 
 case  SALINITY:
 // Serial.print("SALINITY:"); 
 // Serial.println(sensorvalue); 
 break; //DEBUG 
 
 case  PH:
 //Serial.print("PH:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG 
 
 case  TURBIDTY:
 MyPlot.SendData("Turbidity (V)", log_time,  sensorvalue);
 //Serial.print("TURBIDTY:"); 
 //Serial.println(sensorvalue); 
 break; //DEBUG */


