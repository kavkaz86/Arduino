#define BATTERY_MIN 688


void setup() {
  
  Serial.begin(9600);
}


void loop() {
  
  int sensorValue = analogRead(A0);
  
  if(sensorValue > BATTERY_MIN){
    
   
    float voltage = sensorValue * (5.0 / 1023.0);
    Serial.println(voltage);
    delay(10000); 
    
  }
  
}
