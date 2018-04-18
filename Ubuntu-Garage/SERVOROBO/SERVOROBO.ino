/*
// http://playground.arduino.cc/Learning/PhotoResistor
*/
#include <Servo.h>

int lightDetectLeftPin = 2; //d:2
int lightDetectRightPin = 3; //d:3

int whiteLEDleft = 6;
int whiteLEDright = 5;
int buzzer = 4;

int sensorLeft = 0;
int sensorRight = 0;
int thresholdLeft = 700; //d: 720
int thresholdRight = 700;  //d:750
int whiteLeft = 1000;
int whiteRight = 1000;
int blackLeft = 0;
int blackRight = 0;

Servo servoLeft; 
Servo servoRight;
int servoLeftSpeed = 0;
int servoRightSpeed = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(whiteLEDleft, OUTPUT);
    pinMode(whiteLEDright, OUTPUT);
    pinMode(buzzer, OUTPUT);
    servoLeft.attach(13); //d:12 
    servoRight.attach(12);   //d:13
    delay(1000); 
  
    digitalWrite(whiteLEDleft, HIGH);  
    digitalWrite(whiteLEDright, HIGH);  
    delay(1000);
    blackLeft = analogRead(lightDetectLeftPin);
    blackRight = analogRead(lightDetectRightPin);
    delay(500);
    digitalWrite(whiteLEDleft, LOW);  
    digitalWrite(whiteLEDright, LOW);  
    delay(3000);

    digitalWrite(whiteLEDleft, HIGH);  
    digitalWrite(whiteLEDright, HIGH);  
    delay(1000);
    whiteLeft = analogRead(lightDetectLeftPin);
    whiteRight = analogRead(lightDetectRightPin);
    thresholdLeft = (whiteLeft + blackLeft)/2 ;
    thresholdRight = (whiteRight + blackRight)/2;
  
    servoSpeed(100,100);
}

void printStatus(String comment="")
{
    Serial.print("left = ");
    Serial.print(sensorLeft);
    Serial.print(" (white=");
    Serial.print(whiteLeft);
    Serial.print(" black=");
    Serial.print(blackLeft);
    Serial.print(") right = ");
    Serial.print(sensorRight);
    Serial.print(" (white=");
    Serial.print(whiteRight);
    Serial.print(" black=");
    Serial.print(blackRight);
    Serial.print(") ");
    Serial.println(comment);
}

void servoSpeed(int left, int right)
{
    servoLeft.writeMicroseconds(1500+left);
    servoRight.writeMicroseconds(1500-right);
}

void servoStop()
{
    servoLeft.writeMicroseconds(1500); 
    servoRight.writeMicroseconds(1500);
}

void loop() 
{
    //delay(20);
    sensorLeft = analogRead(lightDetectLeftPin);
       
    //delay(20);  
    sensorRight = analogRead(lightDetectRightPin);

    if ((sensorLeft < thresholdLeft) && (sensorRight >= thresholdRight))
    { 
          printStatus("Veer left   ");
          servoSpeed(0,-55);
          delay(100);
          servoSpeed(20,-45);
          delay(50);
    }
    else if ((sensorLeft >= thresholdLeft) && (sensorRight < thresholdRight))
    { 
          printStatus("Veer right  ");
          servoSpeed(-55,0);
          delay(100);         
          servoSpeed(-45,20);
          delay(50);
          
          
    }
    else if ((sensorLeft < thresholdLeft) && (sensorRight < thresholdRight))
    { // Black bar = Stop moving
          printStatus("Black stop  ");
          servoStop();
          digitalWrite(whiteLEDleft, LOW); 
          digitalWrite(whiteLEDright, LOW); 
          //beep(3);
          //delay(1000);
          digitalWrite(whiteLEDleft, HIGH); 
          digitalWrite(whiteLEDright, HIGH);
          //delay(1000); 
    }
    else
    { // White on both sides = Travel straight
          //printStatus("Go straight ");
          servoSpeed(100,100);
    }
}




