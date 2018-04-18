#include <SoftwareSerial.h>
//#include <OneWire.h>


#define  END_OF_TX  '\r'
#define  TX_START   'O'
#define  TX_DEL     ':' 


#define  R       'A'
#define  C       'B'
#define  L1      'C'
#define  L0      'D'
#define  TOGGLE  '%'
#define  E       'E'
#define  TR      'F'


#define  R_CMD       "R"
#define  C_CMD       "C"
#define  L1_CMD      "L1"
#define  L0_CMD      "L0"
#define  TOGGLE_CMD  "%"
#define  E_CMD       "E"
#define  END         "\r"


SoftwareSerial doport(4,5);

String  tx="";
String  do_cmd="";
char  rx[1];
boolean  rx_complete = false;
boolean tx_complete = false;
boolean rx_event = false; 



void setup(){
  
  Serial.begin(9600); 
  
  doport.begin(38400); 
  
  tx.reserve(30);
  do_cmd.reserve(10); 
   
}

void serialEvent(){
  
  char inchar = (char)Serial.read(); 
  if( (inchar == R) || (inchar == C) || (inchar == L1) || (inchar == L0) || (inchar == TOGGLE) || (inchar == E) || (inchar == TR) ){
    rx_event = true;
    rx[0] = inchar;
  }
  if( inchar == END_OF_TX ){
    rx_complete = true;
  }
}  
  
  

void loop(){
  
  if(rx_event && rx_complete){
    uint8_t  req_mode = 0; 
    switch(rx[0]){
      
      case  R:
        do_cmd += R_CMD;
        req_mode = 1;  
        break;
      
      case  C:
        do_cmd += C_CMD;
        req_mode = 1; 
        break; 
       
      case  L1:
        do_cmd += L1_CMD;
        break;
      
      case  L0:
        do_cmd += L0_CMD;
        break; 
       
      case  TOGGLE:
        do_cmd += TOGGLE_CMD;
        break;
       
      case  E:
        do_cmd += E_CMD;
        break;
      
      default: break;  
      
    }
    
    do_cmd += END;
    process_do_req(req_mode);
  }
  /***************** OLD CODE *****************
  
  
          tx +=  TX_START;
          tx +=  TX_DEL; 
          
          do_cmd += "R"; 
          do_cmd += "\r"; 
          
          doport.print(do_cmd); 
          
          delay(200); 
          
          while(doport.available()){
            
            char inchar = (char)doport.read();
            tx += inchar ;
            if(inchar = '\r'){ tx_complete = true;} 
          }
          
          if(tx_complete){
            
            Serial.print(tx); delay(1000); 
            tx= "";
            do_cmd = ""; 
            tx_complete = false; 
  } 
  
  ******************************************/
  
  process_do_req(1); 
  delay(5000);
}

void  process_do_req(uint8_t  req){
  
  if(req == 1){
    tx +=  TX_START;
    tx +=  TX_DEL;
  }
  
  if(req == 1 && !rx_event){
    do_cmd += R_CMD;
    do_cmd += END;
  }
        
  doport.print(do_cmd);
  
  delay(200);
  
  while(doport.available()){
    
    char inchar = (char)doport.read();
    tx += inchar ;
    if(inchar = '\r'){ tx_complete = true;} 
  }
  
  if(tx_complete){
    
    Serial.print(tx); delay(1000); 
    tx= "";
    do_cmd = ""; 
    tx_complete = false;
    if(rx_event){ rx_event= false; } 
  }
 
}
 
   
  
   


