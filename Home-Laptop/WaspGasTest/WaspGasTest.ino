#include <WaspSensorGas.h>

#include "Wire.h"

void setup(){
  
  Serial.begin(9600); 
  SensorGas.setBoardMode(SENS_ON);
  
  SensorGas.configureSensor(SENS_SOCKET3C, 1, 8); 
  SensorGas.configureSensor(SENS_SOCKET4A, 1, 7); 
  SensorGas.setSensorMode(SENS_ON, SENS_SOCKET4A); 
  //pinMode(9, OUTPUT);
  //digitalWrite(9, HIGH);
  
}

void loop(){
  
  //Serial.println(SensorGas.readValue(SENS_TEMPERATURE)); 
  
  //float temperature = SensorGas.readValue(SENS_TEMPERATURE) * 5 / 1023; //sensor output (mV)
  
  //float temperature = 
  //float temperatureC = (SensorGas.readValue(SENS_TEMPERATURE) - 500) / 10;  
  //Serial.println(analogRead(A0)); 
  
  //loat temp =(SensorGas.readValue(SENS_TEMPERATURE) - 500)/(10);
  //Serial.println(temp); 
  float temp = SensorGas.readValue(SENS_TEMPERATURE);
  float tempC= ((temp*5/1023) -0.5) / 0.01 ; 
  Serial.print("ADC= "); Serial.println(temp);
  Serial.print("TempC= "); Serial.println(tempC); 
  
  delay(2000); 
  
}

