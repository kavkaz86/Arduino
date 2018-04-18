#include "MegunoLink.h" // Helpful functions for communicating with MegunoLink Pro. 
#include <SoftwareSerial.h>


#define  TEMP_1    "T1:"     
#define  TEMP_2    "T2:" 
#define  UV        "UV:" 
#define  PAR       "PAR:" 
#define  OXYGEN    "OX:" 
#define  TURBIDTY  "TUR:" 
#define  SAL       "SAL:" 
#define  PH        "PH:" 


SoftwareSerial mySerial(2,3);
// Millis value when the data was last sent. 
long LastSent;

// Interval (milliseconds) between sending analog data
const unsigned SendInterval = 200; // [ms]
String sensorStringdo ="";
char dobuffer[10]; 
boolean input_stringcomplete = false;                                          //have we received all the data from the PC
boolean sensor_stringcomplete = false; 
// The plot we are sending data to. A TimePlot is used here 
XYPlot MyPlot;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  delay(5000); 
 // mySerial.print("6"); 
  LastSent = millis();
  
  MyPlot.SetTitle("Sensor Measurements");
  MyPlot.SetXlabel("Time");
  MyPlot.SetYlabel("Value");
  MyPlot.SetSeriesProperties("Oxygen (MG/L)", Plot::Magenta, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("Temperature (C)", Plot::Red, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("Turbidity (V)", Plot::Blue, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("UV", Plot::Green, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("PAR", Plot::Yellow, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("UV", Plot::Green, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("Salinity", Plot::Black, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("pH", Plot::Cyan, Plot::Solid, 2, Plot::Square);
}

void loop()
{
  
  
  mySerial.print("1");
  delay(300); 
  while(mySerial.available()){
    
    char inchar = (char)mySerial.read();
    sensorStringdo += inchar;
    if(inchar == '\r') {input_stringcomplete = true;}  
  
  }
  
  if (input_stringcomplete){
    LastSent = millis();
    if(sensorStringdo.startsWith(TEMP_1)){}
    MyPlot.SendData("Oxygen (MG/L)", millis(),sensorStringdo); //millis() goes on the xaxis, DataValue goes on the yaxis
    input_stringcomplete = false;
   sensorStringdo = ""; 
  }
  
  delay(2000);
}

