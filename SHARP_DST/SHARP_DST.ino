#define DELAY 5

float distance=0; 

float val = 0;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(A0, INPUT);
  
}

void loop() {

  Serial.println(28250/( analogRead(A0) - 229.5));

  delay(DELAY);

  

}
