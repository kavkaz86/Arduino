#include <WaspSensorGas.h>
#include <Wire.h>

void setup(){
  Serial.begin(9600);
  SensorGas.setBoardMode(SENS_ON);
}

void loop(){
  
  float temp=SensorGas.readValue(SENS_TEMPERATURE);
  float TempC= ((5*temp/1023)-0.5)/0.01 ; 
  
  Serial.print("ADC= "); Serial.println(temp);
  Serial.print("TempC= "); Serial.println(TempC);
  Serial.println("-----------------------"); 
  
  delay(2000);

}
