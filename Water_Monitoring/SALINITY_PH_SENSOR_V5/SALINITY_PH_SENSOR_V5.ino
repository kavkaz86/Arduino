/*   ARDUINO MEGA INTERRUPT AND SERIAL PORT INFO: 
 
 Serial: 0 (RX) and 1 (TX); Serial 1: 19 (RX) and 18 (TX); Serial 2: 17 (RX) and 16 (TX); Serial 3: 15 (RX) and 14 (TX). 
 Used to receive (RX) and transmit (TX) TTL serial data. Pins 0 and 1 are also connected to the corresponding pins 
 of the ATmega16U2 USB-to-TTL Serial chip.
 
 External Interrupts: 2 (interrupt 0), 3 (interrupt 1), 18 (interrupt 5), 19 (interrupt 4), 20 (interrupt 3), and 21 (interrupt 2).
 These pins can be configured to trigger an interrupt on a low value, a rising or falling edge, or a change in value. 
 See the attachInterrupt() function for details
 
 */

/*        RX IDENTIFIER            */
#define DEL        ':'                                                               
#define SAL_ID     'S'                                                               
#define PH_ID      'P'                                                               
#define END_OF_TX  '\r'

/*        SENSOR COMMAND IDs       */
#define  R       'A'
#define  C       'B'
#define  L1      'C'
#define  L0      'D'
#define  E       'E'

/*        SENSOR  CMDs             */
#define  R_CMD    "R"
#define  C_CMD    "C"  
#define  L1_CMD   "L1"
#define  L0_CMD   "L0"
#define  E_CMD    "E"
#define  END      "\r"


/*        TX FRAMES DELIMITER       */
#define  S_TX  "S:"
#define  P_TX  "P:"


/*        GLOBAL VARIABLES          */
String  tx=""; 
String  cmd="";
String  transparent_cmd = "";
char  rx[1];
char  rx_cmd[1];
boolean  tx_complete = false; 
boolean  req_ack= false;
boolean  rx_xbee_complete = false;
boolean  start_xbee_cpy = false;
boolean  finish_xbee_ps = false;

boolean  rx_sal_complete = false;
boolean  tx_sal_complete = false;
const int  XBEE_RX = 19; 
volatile  boolean  rx_event  = false;   


/* Communication is initialised in this function:  
 * 1- Hardware Serial: Serial.begin(BPS)
 * 2- Salinity Serial: salinity.begin(BPS)
 * 3- PH Serial: ph.begin(BPS)
 */
void setup(){

  Serial.begin(9600); //debug  
  delay(200); 
  Serial1.begin(9600); //xbee
  delay(200);
  Serial2.begin(38400); //Salinity
  delay(200);
  Serial3.begin(9600); //ph  
  delay(200);
  attachInterrupt(XBEE_RX, xbee_rx, LOW);  
  tx.reserve(30);
  cmd.reserve(10);  
}

void xbee_rx(){

  rx_event = true;
  detachInterrupt(XBEE_RX);

}

void serialEvent1(){

  char inchar = (char)Serial1.read();
  if( (inchar == SAL_ID) || (inchar == PH_ID) ){
    rx[0] = inchar; 
  }
  if( (inchar == END_OF_TX) ){
    rx_xbee_complete = true; 
    start_xbee_cpy = false; 
  }
  if( (inchar == DEL) ){
    start_xbee_cpy = true; 
  }
  if (start_xbee_cpy && (inchar!=DEL) ){
    rx_cmd[0] = inchar; 
  }
}

//Salinity Port
void serialEvent2(){  
  char  inchar  = (char)Serial2.read();
  tx += inchar ;
  if(inchar = '\r'){
    tx_complete = true;  
    if(req_ack){
      Serial1.print(tx); 
      delay(500);
    }    
  } 
}

//ph Port
void serialEvent3(){  
  char  inchar  = (char)Serial3.read();
  tx += inchar ;
  if(inchar = '\r'){
    tx_complete = true;  
    if(req_ack){
      Serial1.print(tx); 
      delay(500);
    }    
  } 
}

void loop(){

  if(rx_xbee_complete){ 
    req_ack = process_rx_frame(rx_cmd[0]);
    if(rx[0] == SAL_ID){
      if(req_ack){
        tx += S_TX; 
      }
      tx_out(1); 
    }
    else if (rx[0] == PH_ID){
      if(req_ack){
        tx += P_TX; 
      }
      tx_out(2);
    }
    finish_xbee_ps = true; 
  }  
  
  if(tx_complete){
    tx = ""; 
    cmd = "";
    req_ack = false;
    tx_complete = false; 
  } 

  if(rx_event && finish_xbee_ps){
    attachInterrupt(XBEE_RX, xbee_rx, LOW);
    rx_event = false;
    finish_xbee_ps = false;
  }  
}

boolean  process_rx_frame(char  cmd_rx){
  boolean  ack_req = false; 
  switch  (cmd_rx){

  case  R:
    cmd += R_CMD;
    ack_req = true; 
    break;

  case  C:
    cmd += C_CMD;
    ack_req = true; 
    break;

  case  L1:
    cmd += L1_CMD;
    ack_req = false; 
    break;

  case  L0:
    cmd += L0_CMD;
    ack_req = false;
    break;

  case E:
    cmd += E_CMD;
    ack_req = false;
    break;

  default: 
    break;
  }
  cmd +=END;
  return(ack_req); 
}  


void  tx_out(uint8_t  req){

  if(req == 1){
    Serial2.print(cmd);
  }
  else if(req == 2){
    Serial3.print(cmd);
  }
  delay(200);
}


/*
  if(req == 1){
 
 salinity.print(cmd); delay(200);
 if(tx_ok){
 while(salinity.available()){
 
 char inchar = (char)salinity.read();
 tx += inchar ;
 if(inchar = '\r'){ tx_complete = true;} 
 }
 }
 }
 if(req == 2){
 
 ph.print(cmd); delay(200);
 if(tx_ok){
 while(ph.available()){
 
 char inchar = (char)ph.read();
 tx += inchar ;
 if(inchar = '\r'){ tx_complete = true;} 
 }
 }
 }
 
 if(tx_complete){
 
 Serial.print(tx); delay(1000);     
 cmd   =  ""; 
 tx    =  ""; 
 tx_complete = false;
 rx_event= false;
 } */
/*

 void transparent_mode(uint8_t sensor_select){
 //boolean  frame_complete = false; 
 cmd += R_CMD;
 cmd +=END;
 transparent_cmd += E_CMD;
 transparent_cmd += END;
 if(sensor_select == 1){
 tx+=S_TX;
 salinity.print(transparent_cmd);
 delay(2000);
 salinity.listen();
 salinity.print(cmd);
 delay(200);
 while(salinity.available()){
 
 char inchar = (char)salinity.read();
 tx += inchar ;
 if(inchar = '\r'){ tx_complete = true;} 
 }
 }
 else if(sensor_select == 2){
 tx+=P_TX;    
 ph.print(transparent_cmd);
 delay(2000);
 ph.listen(); 
 ph.print(cmd); 
 delay(200);
 while(ph.available()){        
 char inchar = (char)ph.read();
 tx += inchar ;
 if(inchar = '\r'){ tx_complete = true;} 
 }
 }
 
 if(tx_complete){
 
 Serial.print(tx); delay(1000);     
 cmd   =  ""; 
 tx    =  "";
 transparent_cmd = ""; 
 tx_complete = false;   
 }
 } */


/* The main application which deals with incoming and outgoing data
 * is run in this function.
 */
/*void loop(){
 
 if(rx_event){
 boolean  req_ack= false; 
 
 req_ack = process_rx_frame(rx_cmd[1]);
 
 switch(rx[0]){
 
 case SAL_ID:
 if(req_ack){
 tx+=S_TX;
 }
 salinity.listen();       
 tx_out(1, req_ack);
 break;
 
 case PH_ID:
 if(req_ack){
 tx+=P_TX;
 }
 ph.listen();
 tx_out(2, req_ack); 
 break;  
 
 default:  break;              
 }
 }
 
 else{
 ph.listen();
 for(int i =0 ; i< 5 ; i++){
 cmd += R_CMD;
 cmd +=END;
 
 tx+=P_TX;
 
 ph.print(cmd); 
 delay(200);
 while(ph.available()){        
 char inchar = (char)ph.read();
 tx += inchar ;
 if(inchar = '\r'){ tx_complete = true;} 
 }
 if(tx_complete){             
 Serial.print(tx); delay(1000);     
 cmd   =  ""; 
 tx    =  ""; 
 tx_complete = false; 
 }
 delay(5000);
 }
 salinity.listen(); 
 for(int i =0 ; i< 5 ; i++){
 cmd += R_CMD;
 cmd +=END;
 
 tx+=S_TX;
 
 salinity.print(cmd); 
 delay(200);
 while(salinity.available()){        
 char inchar = (char)salinity.read();
 tx += inchar ;
 if(inchar = '\r'){ tx_complete = true;} 
 }
 if(tx_complete){          
 Serial.print(tx); delay(1000);     
 cmd   =  ""; 
 tx    =  ""; 
 tx_complete = false; 
 }
 delay(5000);
 }
 
 
 //delay(8000);  
 }
 */







/*
  if(req == 1){
 tx +=  TX_START;
 tx +=  TX_DEL;
 }
 
 if(req == 1 && !rx_event){
 do_cmd += R_CMD;
 do_cmd += END;
 } */
/*        
 if(tx_complete){
 
 Serial.print(tx); delay(1000); 
 mode  =  0;
 cmd   =  ""; 
 tx    =  ""; 
 tx_complete = false;
 if(rx_event){ rx_event= false; } 
 }
 
 } */

/* This function is invoked when the RX line is pulled LOW 
 * signalling data is available.
 */
/*void serialEvent(){  
 
 char inchar = (char)Serial.read();                                 
 if( (inchar == SAL_ID) || (inchar == PH_ID) ){
 rx[0] = inchar;
 }
 
 if( inchar == END_OF_TX){
 rx_event = true;
 start_cpy = false; 
 }
 
 if( inchar == DEL ){
 start_cpy = true;
 }
 
 if(start_cpy && inchar!=DEL){
 rx_cmd[1] = inchar; 
 }
 
 } */






