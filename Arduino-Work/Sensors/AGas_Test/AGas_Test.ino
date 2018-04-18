#include <WaspSensorGas.h>
#include <Wire.h>

void setup(){
  Wire.begin();
  Serial.begin(9600);
  SensorGas.setBoardMode(SENS_ON);
  SensorGas.configureSensor(SENS_SOCKET4A, 1, 40); 
  SensorGas.setSensorMode(SENS_ON, SENS_SOCKET4A);
  SensorGas.configureSensor(SENS_SOCKET3C, 1, 40);
  SensorGas.configureSensor(SENS_SOCKET2B, 1, 5);
  SensorGas.setSensorMode(SENS_ON, SENS_SOCKET2B);
  //SensorGas.configureSensor(SENS_O2, 100);
  //SensorGas.setSensorMode(SENS_ON, SENS_O2);
  


}

void loop(){
  
  float Gas = SensorGas.readValue(SENS_SOCKET4A); 
 
  float Gas_v= 5*Gas/1023;  
  
  float Temp = SensorGas.readValue(SENS_TEMPERATURE); 
  
  float Temp_v = (((5*Temp)/1023)-0.5)/0.01;
  
  //float O2 = SensorGas.readValue(SENS_O2);
  
  //float O2_v = 5*O2/1023;
  
  //float O2_c = (((O2_v)*10) - 0.4)*(25/8); 
  
  float NH3 = SensorGas.readValue(SENS_SOCKET3C);
  
  float NH3_v = 5*NH3/1023; 
  
  float O3 = SensorGas.readValue(SENS_SOCKET2B);
  
  //float O3_v = 5*O3/1023; 
  
  Serial.print("ADC_CH4= "); Serial.println(Gas);
  Serial.print("Voltage_CH4= "); Serial.println(Gas_v);
  Serial.print("ADC_NH3= "); Serial.println(NH3);
  Serial.print("Voltage_NH3= "); Serial.println(NH3_v);
  Serial.print("ADC_Temp= "); Serial.println(Temp);
  Serial.print("Voltage_Temp= "); Serial.println(Temp_v);
  //Serial.print("ADC_O2= "); Serial.println(O2);
  //Serial.print("Voltage_O2= "); Serial.println(O2_v);
  //Serial.print("Concentration_O2= "); Serial.println(O2_c);
  Serial.print("ADC_O3= "); Serial.println(O3);
  Serial.print("Voltage_O3= "); Serial.println(O3_v);
  Serial.println("---------------");
  
  delay(5000);


}
