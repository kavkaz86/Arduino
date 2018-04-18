long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void setup(){
  analogReference(INTERNAL);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); 
  
}

void loop(){
  
  long millivolts = readVcc();
  long measured = analogRead(A0);
  long voltage = millivolts * measured / 1023; // answer is in millivolts
  Serial.println(voltage, DEC); 
  
  delay(1000); 
  
}


