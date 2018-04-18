#include <SoftwareSerial.h>                                                    


#define SALINITY_RX 4                                                                
#define SALINITY_TX 5                                                               
#define PH_RX 6                                                               
#define PH_TX 7                                                               
#define SALINITY_SELECT  1
#define PH_SELECT  2 

SoftwareSerial salinity(SALINITY_RX, SALINITY_TX);                                         

SoftwareSerial ph(PH_RX, PH_TX);

String inputstring = "";                                                       
String sensorstring = "";                                                      
boolean input_stringcomplete = false;                                         
boolean sensor_stringcomplete = false;
boolean salinity_sensor = false;
boolean ph_sensor = false;
boolean salinity_detect= false;
boolean ph_detect= false;
int sensor_select = 0; 


void setup(){                                                                
  Serial.begin(9600);                                                      
  salinity.begin(38400);                                                   
  ph.begin(38400);
  inputstring.reserve(5);                                                   
  sensorstring.reserve(30);                                                 
}

void serialEvent(){  

  char inchar = (char)Serial.read();                                 
  inputstring += inchar;
  if(inchar == 'S'){
    salinity_detect = true;
    sensor_select = 1;
  }
  if(inchar == 'P'){
    ph_detect = true;
    sensor_select = 2;
  }  
  if(inchar == '\r') {
    input_stringcomplete = true;
  }                

}  



void loop(){                                                                  

  if (input_stringcomplete){
    
    switch(sensor_select){
  
      case  SALINITY_SELECT:
        Serial.println("Salinity Sensor Selected");       
        salinity_sensor = true;
        ph_sensor = false; 
        break;
      
      case  PH_SELECT:
        Serial.println("pH Sensor Selected");
        ph_sensor = true;
        salinity_sensor = false;
        break;
     
      default:
        break;
    }
  }

  if (SALINITY_SELECT){
    salinity.print(inputstring);      
    salinity.print('\r');
    inputstring = "";                                                        
    input_stringcomplete = false;
    
    while (myserial.available()) {                                              
    char inchar = (char)salinity.read();                                  
    sensorstring += inchar;                                              
    if (inchar == '\r') {
      sensor_stringcomplete = true;
      }                  
    }    
  }



  while (myserial.available()) {                                              
    char inchar = (char)myserial.read();                                  
    sensorstring += inchar;                                              
    if (inchar == '\r') {
      sensor_stringcomplete = true;
    }                  
  }


  if (sensor_stringcomplete){                                                 
    Serial.print(sensorstring);                                           
    sensorstring = "";                                                     
    sensor_stringcomplete = false;                                         
  }
}




