#define pinDelay 50
#define newPinDelay 10



int ledPinRed =  11;
int ledPinGreen = 12;


int counter = 0; 
boolean c_timer = false; 
int ctimer = 0;


declareTaskLoop(initSensor);

taskLoop(initSensor)
{
  
  while(c_timer==true){
    
    digitalWrite(11, HIGH); 
    
    delay(50);
    
    c_timer = false; 
   
  }  
}

taskLoop(readSensor)
{
  
 while(c_timer==false){
   
   digitalWrite(11, LOW);
   
   delay(50);
   
   c_timer = true;
   
   
 }
 
}

taskLoop(act)
{
  
  if(c_timer == false){
    
    digitalWrite(12, HIGH);
    
  }
  
  if(c_timer == true){
    
    digitalWrite(12, LOW);
  }
   
}


  

// The setup() method runs once, when the sketch starts

void setup()   
{                
  // Initialize the digital pins as outputs:
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  
  createTaskLoop(act, NORMAL_PRIORITY);
  createTaskLoop(initSensor, NORMAL_PRIORITY); 
  createTaskLoop(readSensor, NORMAL_PRIORITY);

}


void loop()                     
{ 
  
  
  nextTask(); 
  
}


