#include <Streaming.h>

#include <AESLib.h>


#define  data  "9301448693014486"  
                                              
const uint8_t key[] = {
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

String sensor = "";

char aesinput[17];
int n, pos, counter, req_frame, frame_size, frame_modulo = 0;

void setup(){

  Serial.begin(115200);
  sensor.reserve(100); 
}

void loop(){

  frame_size = sizeof(data) - 1;
  frame_modulo = frame_size % 16;
  
  if(frame_modulo == 0){
    req_frame = frame_size / 16;
  }
  else{
    req_frame = 1+(frame_size / 16);
  }
  
  while(req_frame >= 1){  

    if(frame_modulo!=0 && req_frame ==1){
      for(int i = pos; i<pos+frame_modulo; i++){
        aesinput[counter] = data[i];
        counter++;
      }
      for(int i= frame_modulo; i< 16; i++){
        aesinput[i] = '\0';
      }
    } 
    else{

      for(int i = pos; i < pos+16; i++){      
        aesinput[counter] = data[i];
        counter++;
      }
    }

    for(int i=0; i < (sizeof(aesinput)); i++){
      aesinput[i]= '\0';
    }
    
    req_frame--;
    n++;
    pos = 16*n;
    counter=0;

  }
  pos = 0; 
  n = 0;

}








