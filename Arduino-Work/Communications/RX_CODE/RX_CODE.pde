
char input[]=""; 

int serInLen = 25;

char serInString[25];

int pId =0;

int result;

char buffer[5];

//char Message[6]="HELLO"; 


#define Message "1" 

void setup(){
  
  //Serial.begin(9600,0); 
  
 XBee.begin(0,9600);
 XBee.setMode(XBEE_ON);
    
   
    
}

void loop(){
  
  
  
  if (XBee.available()){ 
  
    char* paq_sent=(char*) calloc(1,sizeof(XBee.available()));  
  
      for(int f=0 ; f < XBee.available() ; f++){
        
        paq_sent[f]= (char)XBee.read();
       
        XBee.print(paq_sent[f]);  
        //input[f] = (char)XBee.read(); 
        //itoa(XBee.read(), &input[f], 10); 
        
       // XBee.print(input[f]); 
      }
      if(Utils.strCmp(paq_sent, Message, 1) == 0){
     
       Utils.blinkLEDs(1000); 
       
      }
      free(paq_sent);
      paq_sent=NULL;
      
    }
    
   
    
    
    
    /*
    for(int x = 0 ; x < Utils.sizeOf(Message) ; x++){
      
      if(input[x] == Message[x]) Utils.blinkLEDs(1000);
    }*/
    
      
      
   
     
   
   
}
  

