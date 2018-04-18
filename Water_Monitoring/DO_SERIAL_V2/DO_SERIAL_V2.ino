/*

THINGS TO DO: - 

  1. ADD SLEEPING OPTINOS TO SAVE POWER
  2. FIX CONT MODE

*/

#include <SoftwareSerial.h>
#include <OneWire.h>

SoftwareSerial doport(4,5);
OneWire  ds(10);  


// DO CIRCUIT FUCNTIONS
#define  NON_CONTINUOUS    49  // ASCII 1 
#define  CONTINUOUS        50  // ASCII 2
#define  STOP_DETECTION    51  // ASCII 3
#define  CALIBRATION       52  // ASCII 4
#define  FACTORY_RESET     53  // ASCII 5
#define  TOGGLE            54  // ASCII 6
#define  T_COMP            55  // ASCII 7
#define  S_COMP            56  // ASCII 8
#define  B_COMP            57  // ASCII 9

//  DO COMMAND LIST
#define  TX_CMD1           "r" // NON_CONTINUOUS
#define  TX_CMD2           "c" // CONTINUOUS
#define  STOP_CMD          "e" // STOP_DETECTION
#define  CALIBRATE_CMD     "m" // CALIBRATION
#define  RESET_CMD         "x" // FACTORY RESET
#define  TOGGLE_CMD        "%" // TOGGLE

// GET_OXYGEN PARSING
#define  CMD_1              1  // NON_CONTINUOUS
#define  CMD_2              2  // CONTINUOUS 
#define  CMD_3              3  // STOP_DETECTION 
#define  CMD_4              4  // CALIBRATION
#define  CMD_5              5  // FACTORY RESET
#define  CMD_6              6  // TOGGLE
#define  CMD_7              7  // TEMPRATURE COMPENSATED DO


//DS18B20 TEMPERATURE SENSOR
#define  DS18S20_ID 0x10
#define  DS18B20_ID 0x28
#define  CONDUCTIVITY  0

String sensorStringdo ="";
boolean do_string = false;
boolean CONTINOUS_MODE = false;
char dobuffer[10]; 
float  temp  =  0; 

void setup(){
  
  Serial.begin(9600);
  
  doport.begin(38400);
  
}
  
void serialEvent(){
  
  char inChar = (uint8_t)Serial.read();
  
  doport.print(STOP_CMD); doport.print("\r");
  if(CONTINOUS_MODE == true){
    CONTINOUS_MODE = false;
  }
  
  switch(inChar){
      
      case  NON_CONTINUOUS:
        get_Oxygen(1);
        break;
        
      case  CONTINUOUS:
        CONTINOUS_MODE = true;
        doport.print(TX_CMD2); doport.print("\r"); 
        //get_Oxygen(2);
        break;
      
      case  STOP_DETECTION:
        get_Oxygen(3);
        break;
      
      case  CALIBRATION:
        get_Oxygen(4);
        break;
      
      case  FACTORY_RESET:
        get_Oxygen(5);
        break;
      
      case  TOGGLE:
        get_Oxygen(6);
        break;
  
      case  T_COMP:
        get_Oxygen(7);
        break;         
       
      default: 
        break;
    }
}
void loop(){
  
  while(CONTINOUS_MODE==true){
    
    while(doport.available()){
    
    sensorStringdo += (char)doport.read();
  
    }
    
    sensorStringdo.toCharArray(dobuffer, 6); 
    String output = sensorStringdo ;
    Serial.println(output);
    sensorStringdo = "";
  }
  
}

void get_Oxygen(uint8_t DO_CMD){
  
  switch(DO_CMD){
    
    case CMD_1:  doport.print(TX_CMD1);
                 break;
    
    /*
    case CMD_2:  doport.print(TX_CMD2);
                 break; */
    
    case CMD_3:  doport.print(STOP_CMD);
                 break;
                 
    case CMD_4:  doport.print(CALIBRATE_CMD);
                 break;
                 
    case CMD_5:  doport.print(RESET_CMD);
                 break;
                 
    case CMD_6:  doport.print(TOGGLE_CMD);
                 break;
                 
    case CMD_7:  if(getTemperature() == true){
                   doport.print(temp); doport.print(","); doport.print(CONDUCTIVITY); doport.print("\r");
                   delay(200);
                 }
                 doport.print(TX_CMD1);
                 break;             
                   
    default:  break;             
                 
  }
  
  doport.print("\r");
  
  doport.listen();
  
  while(doport.available()){
    
    sensorStringdo += (char)doport.read(); 
  }
  
  sensorStringdo.toCharArray(dobuffer, 6); 
  String output = sensorStringdo ;
  
  Serial.println(output);
  sensorStringdo = "";
  
}

boolean getTemperature(){
 
   byte i;
   byte present = 0;
   byte data[12];
   byte addr[8];
   //find a device
   if (!ds.search(addr)) {
   ds.reset_search();
   return false;
   }
   if (OneWire::crc8( addr, 7) != addr[7]) {
   return false;
   }
   if (addr[0] != DS18S20_ID && addr[0] != DS18B20_ID) {
   return false;
   }
   ds.reset();
   ds.select(addr);
   // Start conversion
   ds.write(0x44, 1);
   // Wait some time...
   delay(850);
   present = ds.reset();
   ds.select(addr);
   // Issue Read scratchpad command
   ds.write(0xBE);
   // Receive 9 bytes
   for ( i = 0; i < 9; i++) {
   data[i] = ds.read();
   }
   // Calculate temperature value
   temp = ( (data[1] << 8) + data[0] )*0.0625;
   return true;
}


  
  
  
  
  
  
    
