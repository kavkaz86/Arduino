#include <Streaming.h>

#include <AESLib.h>


#define  data  "9301448693014486930144869301448693014486930144869301448693014486930144869301448693014486930144869301448"  
                                              
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

  //char data[]="91111111111111112222222222222222333333333333333393014486"; 
  //req_frame = 0;
  char indata[100]; 
  frame_size = sizeof(data) - 1;
  frame_modulo = frame_size % 16;
  if(frame_modulo == 0){
    req_frame = frame_size / 16;
  }
  else{
    req_frame = 1+(frame_size / 16);
  }

  Serial << "frame_size=" << frame_size << endl;
  //int req_frame  = (frame_size)/16)+1; 
  Serial << "required number of frames=" << req_frame << endl;
  //int frame_modulo = (frame_size-1) % 16;
  Serial << "frame_modulo:" << frame_modulo << endl; 

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


    Serial << "pos:" << pos << endl;
    Serial << "n:" << n << endl; 
    Serial << "(n*pos)+16:" << pos+16 << endl;
    aes128_enc_single(key, aesinput);
    //indata=aesinput;
    sensor += aesinput;
    Serial << req_frame << "|Encrypted|" << aesinput << endl; 
    aes128_dec_single(key, aesinput);
    //sensor += aesinput;
    Serial << req_frame << "|Decrypted|" << aesinput << endl;   
    //aessize = sizeof(aesinput);
    //Serial << "aessize(no-reset)" << aessize << endl; 
    for(int i=0; i < (sizeof(aesinput)); i++){
      aesinput[i]= '\0';
    }
    //aessize = sizeof(aesinput);
    //Serial << "aessize(no-reset)" << aessize << endl; 
    Serial << "aesinput:" << aesinput << endl;

    req_frame--;
    n++;
    pos = 16*n;
    counter=0;

  }
  pos = 0; 
  n = 0;
  Serial.println(sensor);
  sensor="";
  delay(10000);  

}








