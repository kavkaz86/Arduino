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
#define NODE1            'A'
#define NODE2            'B'
#define NODE3            'C' 
#define NODE4            'D'
#define DELIMITER        ':'

XYPlot MyPlot("Sensors");
SoftwareSerial mySerial(2,3);

boolean serial_tx_cpy = false; 
boolean tx_complete = false; 
boolean ID_flag = false;
String sensorvalue = ""; 
char sensor_ID[6]; 
int  node1_counter = 0;
int  node2_counter = 0;
int  node3_counter = 0;
int  node4_counter = 0;

void setup(){

  //Serial1.begin(9600);
  Serial.begin(9600);
  mySerial.begin(9600);

  delay(5000); 
  sensorvalue.reserve(30);  
  
  MyPlot.SetTitle("Bush Fire Sensor Network");
  MyPlot.SetXlabel("Cycle");
  MyPlot.SetYlabel("Gas (V)");
  MyPlot.SetSeriesProperties("NODE_1", Plot::Magenta, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("NODE_2", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
  MyPlot.SetSeriesProperties("NODE_3", Plot::Black, Plot::Solid, 2, Plot::Triangle);
  MyPlot.SetSeriesProperties("NODE_4", Plot::Blue, Plot::Solid, 2, Plot::Circle);

}



void loop(){


  if(mySerial.available()){

    char inchar = (char)mySerial.read();
    
    if( (inchar == NODE1) || (inchar == NODE2) || (inchar == NODE3) || (inchar == NODE4) ) 
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
    case  NODE1:           
      MyPlot.SendData("NODE_1", node1_counter,  sensorvalue);
      node1_counter++;    
      break;

    case  NODE2:     
      MyPlot.SendData("NODE_2", node2_counter,  sensorvalue); 
      node2_counter++;   
      break; 

    case  NODE3:      
      MyPlot.SendData("NODE_3", node3_counter,  sensorvalue);
      node3_counter++;      
      break; 
      
    case  NODE4:      
      MyPlot.SendData("NODE_4", node4_counter,  sensorvalue);
      node4_counter++;      
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


