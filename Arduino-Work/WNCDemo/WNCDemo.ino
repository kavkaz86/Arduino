#define P1 1
#define P2 2
#define P3 3
#define P4 4

void setup(){
  
  Serial.begin(9600); 
  
}

void loop(){
  Serial.println("Start Program"); 
  Serial.println(""); 
  Serial.print("P1="); Serial.println(P1); delay(200); 
  Serial.println("-----------------------"); delay(200);
  Serial.print("P2="); Serial.println(P2);delay(200);
  Serial.println("-----------------------");delay(200);
  Serial.print("P3="); Serial.println(P3);delay(200);
  Serial.println("-----------------------");delay(200);
  Serial.print("P4="); Serial.println(P4);delay(200);
  Serial.println("-----------------------"); delay(200);
 
  int L = P1^P3^P4^(P1^P2^P3^P4); 
  
  Serial.print("Data Received at Node4 (P2)= "); Serial.println(L);delay(200); 
  Serial.println("-----------------------");delay(200);
  Serial.println("-----------------------"); delay(200);
  Serial.println("End of Program"); 
  Serial.println(""); 
  delay(2000); 
  
}

