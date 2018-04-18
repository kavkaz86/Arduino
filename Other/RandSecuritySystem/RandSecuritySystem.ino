#define Alarm "1" 
int pin = 13;
volatile int state = LOW;
void setup()
{
  
  Serial.begin(9600); 
  pinMode(pin, OUTPUT);
  attachInterrupt(0, blink, RISING);
}

void loop()
{   
    if(state==HIGH){
      state=LOW;
      Serial.println(Alarm); 
    }
}

void blink()
{
  state=HIGH ; 
}
