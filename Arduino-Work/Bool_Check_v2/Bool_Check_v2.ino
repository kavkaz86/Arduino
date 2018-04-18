#include  "Streaming.h"

bool IR_flag[4]={1,0,0,0};
bool c[3] = {0,0,0};

void setup() {
  Serial.begin(9600);

}

void loop() {
  
  setflag(0,0,0); 
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl; 
  delay(500);
  
  setflag(0,0,1);
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl;
  delay(500);
 
  setflag(0,1,0);
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl;
  delay(500);

  setflag(0,1,1);
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl; 
  delay(500);

  setflag(1,0,0);
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl;
  delay(500);

  setflag(1,0,1); 
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl;
  delay(500);

  setflag(1,1,0); 
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl;
  delay(500);

  setflag(1,1,1); 
  Serial << IR_flag[0] << IR_flag[1] << IR_flag[2] << IR_flag[3] << " | check:" << check1(IR_flag) << endl;
  delay(500);

}


void setflag(const bool x, const bool y, const bool z){

   IR_flag[1] = x;
   IR_flag[2] = y;
   IR_flag[3] = z;
}

bool check(const bool array[], int n)
{   
    const bool a0 = array[0];  
    for (int i = 1; i < n; i++)      
    {         
        if (array[i] != a0)
            return true;
    }
    return false;
}

int check1(const bool array[]){

  const bool a0 = array[0]; 

  if(array[1] != a0 && array[2] != a0 && array[3] != a0){
    return 0; 
  }
  else if(array[1] != a0 && array[2] != a0 && array[3] == a0){
    return 1;
  }
  else if(array[1] != a0 && array[2] == a0 && array[3] != a0){
    return 2;
  }
  else if(array[1] != a0 && array[2] == a0 && array[3] == a0){
    return 3;
  }
  else if(array[1] == a0 && array[2] != a0 && array[3] != a0){
    return 4;
  }
  else if(array[1] == a0 && array[2] != a0 && array[3] == a0){
    return 5;
  }
  if(array[1] == a0 && array[2] == a0 && array[3] != a0){
    return 6; 
  }
  if(array[1] == a0 && array[2] == a0 && array[3] == a0){
    return 7; 
  }
 
}

