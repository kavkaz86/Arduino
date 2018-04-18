/*
    GREEN-----------BLUE   
             RED
   YELLOW-----------WHITE        

*/

void setup(){
  pinMode(2,OUTPUT); //RIGHT SSR
  pinMode(3,OUTPUT); // LEFT SSR
  
  pinMode(4,OUTPUT); //GREEN
  pinMode(5,OUTPUT); // WHITE
  pinMode(6,OUTPUT); //RED
  pinMode(7,OUTPUT); //YELLOW
  pinMode(8,OUTPUT); //BLUE

  
}

void loop(){
  
  digitalWrite(2,HIGH);
  delay(2000);
  digitalWrite(2,LOW);
  
  digitalWrite(3,HIGH);
  delay(2000);
  digitalWrite(3,LOW);
  
  digitalWrite(4,HIGH);
  delay(2000);
  digitalWrite(4,LOW);
  
  digitalWrite(5,HIGH);
  delay(2000);
  digitalWrite(5,LOW);
  
  digitalWrite(6,HIGH);
  delay(2000);
  digitalWrite(6,LOW);
  
  digitalWrite(7,HIGH);
  delay(2000);
  digitalWrite(7,LOW);
  
  digitalWrite(8,HIGH);
  delay(2000);
  digitalWrite(8,LOW);
  
}
