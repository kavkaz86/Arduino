#define bat_min 688

#define DELAY 10000

void setup(){
  
  Serial.begin(9600); 
  
}

void loop(){
  
  int sensorValue = analogRead(A0); 
  
  if(sensorValue > bat_min){
    
    float sensorVoltage = 5.0 * sensorValue / 1023.0 ; 
    
    Serial.println(sensorVoltage); 
    
    delay(DELAY); 
  }
  
 
}

