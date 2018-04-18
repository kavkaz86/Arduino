String sensor = "";
String encrypt_sensor = ""; 
void setup(){
  
  Serial.begin(9600); 
  sensor.reserve(100);
  encrypt_sensor.reserve(20); 
}

void loop(){

  char data[]="ABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDE"; 

  int frame_size = sizeof(data); 

  int req_frame  = (frame_size/16) + 1;
  int n = 0;
  int pos = 16*n;
  
  while(req_frame > 0){
    
    for(int i = pos ; i < (pos+16); i++){
      encrypt_sensor+= data[i];
    }
    // Encrypt function encrypt_sensor
    sensor += encrypt_sensor;
    req_frame--;
    n++; 
    pos=16*n;
    encrypt_sensor="";
  }

  Serial.println(sensor);
  delay(1000);  
    
}


