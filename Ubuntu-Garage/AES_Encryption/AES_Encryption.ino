#include <AESLib.h>

//char data[16] = {'S','E','N','S','O','R','=','1','2','.','0','5','\0','\0','\0','\0'}; 

//char data[16] = {'S','E','N','S','O','R','=','1','2','.','0','5','2'}; 
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

void setup(){

  Serial.begin(115200);
     
  
}

void loop(){
  char data[] = "SENSOR=12.05";
  int data_size = sizeof(data); 
  if(data_size < 16){
    
    for(int i=data_size+1; i < 17; i++){
      data[i] = '\0' ;
    }
    
  }
  /*
  if(data_size > 16){
    char data1[]="";
    char data2[]="";
    int num_frames = data_size / 16;
    num_frames += 1; 
    Serial.println(num_frames);
    for(int i =0; i<16; i++){
       data1[i] = data[i];  
    }
    for(int i=17; i<
      
  }*/
  
  
  aes128_enc_single(key, data);
  Serial.print("encrypted:");
  Serial.println(data);
  aes128_dec_single(key, data);
  Serial.print("decrypted:");
  Serial.println(data);
  delay(5000);
  
}



