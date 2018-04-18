#define readSens 1 
#define lowLight 2
#define medLight 3
#define highLight 4

void setup(){
  
  Serial.begin(9600); 
  
  for(int i=2; i<7; i++){
    
    pinMode(i, OUTPUT); 
  }
  
  
}

void loop(){
  
  if(Serial.available()){ 
    
    delay(200);
    
  //  int senseVal = Serial.read(); 
    int SensVal = Serial.read(); 
    switch(SensVal){
      
      case readSens:
   
        int rawSens;

        rawSens = analogRead(A0);
 
        if (rawSens >= 480)
        {
          for(int i=2; i<7; i++){
            digitalWrite(i, HIGH); 
          }
          delay(5000);  
          for(int i=2; i<7; i++){
            digitalWrite(i, LOW); 
          }
        }
   
        break;
        
        case lowLight:
          
  
          digitalWrite(2, HIGH); 
          
          delay(5000); 
          
          digitalWrite(2, LOW); 

          break; 
          
          
        case medLight:

          for(int i=2; i<5; i++){
            digitalWrite(i, HIGH); 
          }
          delay(5000);  
          for(int i=2; i<5; i++){
            digitalWrite(i, LOW); 
          }
         break;
         
         
         case highLight:
        
          for(int i=2; i<7; i++){
            digitalWrite(i, HIGH); 
          }
          delay(5000);  
          for(int i=2; i<7; i++){
            digitalWrite(i, LOW); 
          }
          
         break;
         
         default:
            break;      
        
    }
  }
}
  
        
        
