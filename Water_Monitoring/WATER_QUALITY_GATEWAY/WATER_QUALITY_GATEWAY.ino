/*
**********************************************************************************************
 *                INCLUDE FILES
 **********************************************************************************************
 */
#include "MegunoLink.h"
#include <SoftwareSerial.h>


/*
**********************************************************************************************
 *                LOCAL DEFINES
 **********************************************************************************************
 */
 
/*     SENSOR IDs       */ 
#define TEMP_1    'A'
#define TEMP_2    'B'
#define UV        'C'
#define PAR       'D'
#define OXYGEN    'O'
#define SALINITY  'F'
#define PH        'G'
#define TURBIDTY  'H'
#define DELIMITER  ':'

XYPlot MyPlot;
SoftwareSerial mySerial(2,3);


/*
**********************************************************************************************
 *                EXTERNAL VARIABLES
 **********************************************************************************************
 */
 
boolean serial_tx_cpy = false; 
boolean tx_complete = false; 
boolean ID_flag = false;
String sensorvalue = ""; 
char sensor_ID[1]; 
int  UV_COUNT = 0;
int  PAR_COUNT = 0;
int  TUR_COUNT = 0;
int  TEMP_COUNT = 0;

void setup(){

  Serial.begin(9600);
  mySerial.begin(9600);

  delay(20000); 
  sensorvalue.reserve(30);

/*******************************************************************************************************\
 *                              MEGULINK SERIES INITIATION                                             *
 * Colors = { Red, Green, Blue, Yellow, Black, Magenta, Cyan, White }                                  *
 * LineStyle = { Solid, Dashed, Dotted, DashDot, DashDotDot, NoLine }                                  *
 * MarkerStyle = {Square, Diamond, Triangle, Circle, Cross, Plus, Star, DownwardTriangle, NoMarker }   *
 ******************************************************************************************************/
 
  MyPlot.SetTitle("Sensor Measurements");
  MyPlot.SetXlabel("Time");
  MyPlot.SetYlabel("Value");
  MyPlot.SetSeriesProperties("Oxygen (MG/L)", Plot::Magenta, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("Temperature_1 (C)", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
  MyPlot.SetSeriesProperties("Temperature_2 (C)", Plot::Magenta, Plot::Solid, 2, Plot::Triangle);
  MyPlot.SetSeriesProperties("Turbidity (V)", Plot::Blue, Plot::Dashed, 2, Plot::Circle);
  MyPlot.SetSeriesProperties("UV", Plot::Green, Plot::Solid, 2, Plot::Plus);
  MyPlot.SetSeriesProperties("PAR", Plot::Black, Plot::Dashed, 2, Plot::Star);
  MyPlot.SetSeriesProperties("Salinity", Plot::Black, Plot::Solid, 2, Plot::DownwardTriangle);
  MyPlot.SetSeriesProperties("pH", Plot::Cyan, Plot::Dashed, 2, Plot::Square); 

}



void loop(){


  if(mySerial.available()){

    char inchar = (char)mySerial.read();

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

  if (tx_complete){ 
    switch(sensor_ID[0]){
    case  TEMP_1: 
      TEMP_COUNT++;    
      MyPlot.SendData("Temperature_1 (C)", TEMP_COUNT,  sensorvalue);    
      break;

    case  PAR:
      PAR_COUNT++;
      MyPlot.SendData("PAR", PAR_COUNT,  sensorvalue);    
      break; 

    case  UV:
      UV_COUNT++;
      MyPlot.SendData("UV", UV_COUNT,  sensorvalue);      
      break; 

    case  TURBIDTY:
      TUR_COUNT++;
      MyPlot.SendData("Turbidity (V)", TUR_COUNT,  sensorvalue);      
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
