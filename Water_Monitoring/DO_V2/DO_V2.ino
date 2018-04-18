#include <SoftwareSerial.h>                                                    

/*        SOFTWARE SERIAL PORTS     */ 
#define DO_RX 4                                                                
#define DO_TX 5                                                               


/*        SENSOR COMMAND IDs        */
#define O_STOP_CMD  'A'
#define O_CONT_CMD  'B'
#define O_SING_CMD  'C'


/*        RX ACK CMDS              */
#define NO_ACK  0
#define DO_ACK  1
#define CARRIAGE  "\r"


/*        SENSOR SELECT            */ 
#define  OXYGEN  1


/*        SOFTWARE SERIAL PORT DEFINITIONS        */
SoftwareSerial doxygen(DO_RX, DO_TX);                                         


/*        GLOBAL VARIABLES          */   
const char SENSOR_CMDS[3] = {'E', 'C', 'R'};
uint8_t inputstring[2]; 
String sensorstring = "";
String txstring = "";
boolean input_stringcomplete = false;                                         
boolean sensor_stringcomplete = false;
uint8_t sensor_select = 0;
int serial_tx_CMD = 0;


/* Communication is initialised in this function:  
 * 1- Hardware Serial: Serial.begin(BPS)
 * 2- Salinity Serial: salinity.begin(BPS)
 * 3- PH Serial: ph.begin(BPS)
 */
void setup(){

  Serial.begin(9600);                                                      
  doxygen.begin(38400); delay(200);                                                
  sensorstring.reserve(30);
  txstring.reserve(50);  
}


/* This function is invoked when the RX line is pulled LOW 
 * signalling data is available.
 */
void serialEvent(){  
  
  char inchar = (char)Serial.read();                                 
  
  if ((inchar == O_STOP_CMD) || (inchar == O_CONT_CMD) || 
      (inchar == O_SING_CMD) ) {
    inputstring[0] = inchar;    
    inputstring[1] = '\r'; 
    input_stringcomplete = true;
  }
     
}




/* The main application which deals with incoming and outgoing data
 * is run in this function.
 */
void loop(){
    
  if (input_stringcomplete){  // input_stringcomplete is true when a valid CMD is received from the UART
   
      /* inputstring[0] = 1-byte SENSOR COMMAND ID. 
       * sensor select function: sensor_select_f(SENSOR SELECT, SENSOR_COMMAND, RX_ACK REQUIREMENT)
       * SENSOR COMMAND:  E - STOP ALL MEASUREMENTS
       *                  R - SINGLE MEASUREMENT ONLY
       *                  C - CONT. MEASUREMENT (SAL: 1/sec , PH: 1/500ms)
       */
       
      boolean  rx_ack = false;  
      switch(inputstring[0]){                                                
                                                                               
        case  O_STOP_CMD:                                                     
          rx_ack = false;                                                      
          serial_tx_CMD = sensor_select_f(OXYGEN, SENSOR_CMDS[0], rx_ack);           
          break;                                                                             
            
        case  O_CONT_CMD:
          rx_ack = true;         
          serial_tx_CMD = sensor_select_f(OXYGEN, SENSOR_CMDS[1], rx_ack);            
          break;
          
        case  O_SING_CMD:
          rx_ack = true;        
          serial_tx_CMD = sensor_select_f(OXYGEN, SENSOR_CMDS[2], rx_ack);  
          break;
         
        default:  break;  
      
      }     
      inputstring[0] = NULL; // Clear the first byte of the CMD array                                                        
      input_stringcomplete = false; // Clear flag for next incoming CMD                                            
    }
  
  // Listen to the salinity serial port in order to receive its data. 
  if(serial_tx_CMD == DO_ACK){  
    doxygen.listen();
    while (doxygen.available()) {    
      char inchar = (char)doxygen.read();                               
      sensorstring += inchar;                                              
      if (inchar == '\r') {
        sensor_stringcomplete = true;
      }                   
    }
  }

  // Transmit the data over Radio/PC UART. 
  if (sensor_stringcomplete){
    txstring +="O:";
    txstring += sensorstring;     
   // Serial.print("O:"); Serial.println(sensorstring);      
    Serial.print(txstring); delay(500); 
    txstring = "";    
    sensorstring = ""; // Clear sensor string                                                      
    sensor_stringcomplete = false;  // Clear sensor flag                                         
  }  
    
}

/* Sensor select function returns:
 * 0: NO ACK REQ.
 * 1: SALINITY ACK REQ.
 * 2: PH ACK REQ.
 */
int sensor_select_f(uint8_t sensor_type, char sensor_CMD, boolean ack_rx){
  int  sensor_ack_rtn = 0; 
  if(ack_rx == false){
    sensor_ack_rtn = NO_ACK; 
     doxygen.print(sensor_CMD); doxygen.print(CARRIAGE);               
  }
    
  else{    
    sensor_ack_rtn = DO_ACK;
    doxygen.print(sensor_CMD); doxygen.print(CARRIAGE);
  }
  return(sensor_ack_rtn);
}







