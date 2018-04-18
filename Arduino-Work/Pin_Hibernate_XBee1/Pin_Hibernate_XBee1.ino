#define txmsg "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRST " 


void setup(){
  
  pinMode(7, OUTPUT); // SLEEP_RQ
  
  Serial.begin(9600); //Init Serial
  
  digitalWrite(7, HIGH); //Assert Sleep_RQ
  
}

long previous = 0 ; 

void loop(){

 Tx_msg(txmsg); //Transmit
 
 digitalWrite(7, HIGH); //Sleep 
 
}

//Transmit Functions: -


void Tx_msg(char* data){
  
   previous = millis(); 
 
   while((millis() - previous) < 1000);
    
   digitalWrite(7, LOW); 
   
   delay(14);
   
   Serial.println(data);

   delay(200); 
   
   return; 
}
