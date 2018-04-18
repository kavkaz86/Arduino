#include <SoftwareSerial.h>                                                    

/*        SOFTWARE SERIAL PORTS     */ 
#define SALINITY_RX 4                                                                
#define SALINITY_TX 5                                                               
#define PH_RX 6                                                               
#define PH_TX 7


/*        SENSOR COMMAND IDs        */
#define S_STOP_CMD  65  //A
#define S_CONT_CMD  66  //B
#define S_SING_CMD  67  //C
#define P_STOP_CMD  68  //D
#define P_CONT_CMD  69  //E
#define P_SING_CMD  70  //F


/*        RX ACK CMDS              */
#define NO_ACK  0
#define SA_ACK  1
#define PH_ACK  2
#define CARRIAGE  "\r"


/*        SENSOR SELECT            */ 
#define  SALINITY  1
#define  PH        2


/*        SOFTWARE SERIAL PORT DEFINITIONS        */
SoftwareSerial salinity(SALINITY_RX, SALINITY_TX);                                         
SoftwareSerial ph(PH_RX, PH_TX);


/*        GLOBAL VARIABLES          */   
const char SENSOR_CMDS[3] = {'E', 'C', 'R'};
uint8_t inputstring[2]; 
String sensorstring = "";                                                      
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
  salinity.begin(38400); delay(200);  
  ph.begin(38400); delay(200);                                               
  sensorstring.reserve(30);  
}


/* This function is invoked when the RX line is pulled LOW 
 * signalling data is available.
 */
void serialEvent(){  
  
  uint8_t inchar = (uint8_t)Serial.read();                                 
  
  if ((inchar == S_STOP_CMD) || (inchar == S_CONT_CMD) || 
      (inchar == S_SING_CMD) || (inchar == P_STOP_CMD) || 
      (inchar == P_CONT_CMD) || (inchar == P_SING_CMD)) {
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
                                                                               
        case  S_STOP_CMD:                                                     
          rx_ack = false;                                                      
          serial_tx_CMD = sensor_select_f(SALINITY, SENSOR_CMDS[0], rx_ack);           
          break;                                                                             
            
        case  S_CONT_CMD:
          rx_ack = true;         
          serial_tx_CMD = sensor_select_f(SALINITY, SENSOR_CMDS[1], rx_ack);            
          break;
          
        case  S_SING_CMD:
          rx_ack = true;        
          serial_tx_CMD = sensor_select_f(SALINITY, SENSOR_CMDS[2], rx_ack);  
          break;
        
        case  P_STOP_CMD:
          rx_ack = false;        
          serial_tx_CMD = sensor_select_f(PH, SENSOR_CMDS[0], rx_ack);  
          break;                    
        
        case  P_CONT_CMD:
          rx_ack = true;        
          serial_tx_CMD = sensor_select_f(PH, SENSOR_CMDS[1], rx_ack);  
          break;
          
        case  P_SING_CMD:
          rx_ack = true;        
          serial_tx_CMD = sensor_select_f(PH, SENSOR_CMDS[2], rx_ack);  
          break; 
         
        default:  break;  
      
      }     
      inputstring[0] = NULL; // Clear the first byte of the CMD array                                                        
      input_stringcomplete = false; // Clear flag for next incoming CMD                                            
    }
  
  // Listen to the salinity serial port in order to receive its data. 
  if(serial_tx_CMD == SA_ACK){  
    salinity.listen();
    while (salinity.available()) {    
      char inchar = (char)salinity.read();                               
      sensorstring += inchar;                                              
      if (inchar == '\r') {
        sensor_stringcomplete = true;
      }                   
    }
  }
  // Listen to the ph serial port in order to receive its data.
  else if(serial_tx_CMD == PH_ACK){
    ph.listen();
    while (ph.available()) {                                               
      char inchar = (char)ph.read();                                
      sensorstring += inchar;                                               
      if (inchar == '\r') {
        sensor_stringcomplete = true;
      }                  
    }
  }    
  
  // Transmit the data over Radio/PC UART. 
  if (sensor_stringcomplete){                                                 
    Serial.println(sensorstring);                                           
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
    
    switch(sensor_type){
            
            case  SALINITY:
                  salinity.print(sensor_CMD); salinity.print(CARRIAGE);
                  break;
                  
            case  PH:
                  ph.print(sensor_CMD); ph.print(CARRIAGE);
                  break;
            
            default:  Serial.println("error"); break;      
                  
    }
  }
    
  else{
    switch(sensor_type){
            
            case  SALINITY:
                  sensor_ack_rtn = SA_ACK;
                  salinity.print(sensor_CMD); salinity.print(CARRIAGE);
                  break;
            
            case  PH:
                  sensor_ack_rtn = PH_ACK;
                  ph.print(sensor_CMD); ph.print(CARRIAGE);
                  break;
            
            default:  Serial.println("error"); break; 
    }
  }
  return(sensor_ack_rtn);
}







