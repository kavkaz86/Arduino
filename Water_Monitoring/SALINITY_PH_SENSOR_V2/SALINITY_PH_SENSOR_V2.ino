#include <SoftwareSerial.h>                                                    


#define SALINITY_RX 4                                                                
#define SALINITY_TX 5                                                               
#define PH_RX 6                                                               
#define PH_TX 7            
#define  NO_SELECTION  0
#define  SALINITY_SENSOR  1
#define  PH_SENSOR  2

SoftwareSerial salinity(SALINITY_RX, SALINITY_TX);                                         

SoftwareSerial ph(PH_RX, PH_TX);

String inputstring = "";                                                       
String sensorstring = "";                                                      
boolean input_stringcomplete = false;                                         
boolean sensor_stringcomplete = false;
boolean  write_TX = false;
int sensor_select = 0; 
boolean sensor_detect = false; 

void setup(){

  Serial.begin(9600);                                                      
  salinity.begin(38400);    delay(200);  
  ph.begin(38400);  delay(200); 
  inputstring.reserve(5);                                                   
  sensorstring.reserve(30);  
}
void serialEvent(){  

  char inchar = (char)Serial.read();                                 
  inputstring += inchar;
  if(inchar == 'S' || inchar == 'P'){ sensor_detect = true; }

  if(inchar == '\r') {
    input_stringcomplete = true;
  }                
}

void loop(){


  if (input_stringcomplete){                                                  
    
    if(sensor_detect){
      if (inputstring.startsWith("S")){        
        salinity.listen();
        Serial.println("Salinity Selected"); 
        ph.print("E"); ph.print("\r"); 
        sensor_select = SALINITY_SENSOR;
      }
      else if (inputstring.startsWith("P")){
        ph.listen();
        Serial.println("PH Selected"); 
        salinity.print("E"); salinity.print("\r");     
        sensor_select = PH_SENSOR;
      }
      inputstring = "";  
      input_stringcomplete = false; 
    }
    sensor_write(sensor_select);
    sensor_detect = false; 
      
    /*   
    if (inputstring.startsWith("S")){
      salinity.listen();
      Serial.println("Salinity Selected"); 
      ph.print("E"); ph.print("\r"); 
      sensor_select = SALINITY_SENSOR;
      inputstring = "";  
      input_stringcomplete = false;
    }

    else if (inputstring.startsWith("P")){
      ph.listen();
      Serial.println("PH Selected"); 
      salinity.print("E"); salinity.print("\r");     
      sensor_select = PH_SENSOR;
      inputstring = "";  
      input_stringcomplete = false;     
    } */ 
    

    //else 
     // sensor_write(sensor_select);
  }

  if(sensor_select == SALINITY_SENSOR){
    
    while (salinity.available()) {                                               //while a char is holding in the serial buffer
      char inchar = (char)salinity.read();                                  //get the new char
      sensorstring += inchar;                                               //add it to the sensorString
      if (inchar == '\r') {
        sensor_stringcomplete = true;
      }                   //if the incoming character is a <CR>, set the flag
    }
  }

  else if (sensor_select == PH_SENSOR){    
    while (ph.available()) {                                               //while a char is holding in the serial buffer
      char inchar = (char)ph.read();                                  //get the new char
      sensorstring += inchar;                                               //add it to the sensorString
      if (inchar == '\r') {
        sensor_stringcomplete = true;
      }                  
    }
  }

  if (sensor_stringcomplete){                                                 //if a string from the Atlas Scientific product has been received in its entirety
    Serial.println(sensorstring);                                             //use the hardware serial port to send that data to the PC
    sensorstring = "";                                                      //clear the string:
    sensor_stringcomplete = false;                                          //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }


}


void sensor_write(int sensor_type){


  switch(sensor_type){

  case  SALINITY_SENSOR:
    salinity.print(inputstring);      //send that string to the Atlas Scientific product
    salinity.print('\r');      
    break; 

  case  PH_SENSOR:    
    ph.print(inputstring);      //send that string to the Atlas Scientific product
    ph.print('\r');
    break;

  default:  
    Serial.println("No Sensor Selected"); 
    break;           
  }
  inputstring = "";
  input_stringcomplete = false;   
  
}

