/*
Colors = { Red, Green, Blue, Yellow, Black, Magenta, Cyan, White }
LineStyle = { Solid, Dashed, Dotted, DashDot, DashDotDot, NoLine }
MarkerStyle = {Square, Diamond, Triangle, Circle, Cross, Plus, Star, DownwardTriangle, NoMarker }
*/

#include "MegunoLink.h"
#include <SoftwareSerial.h>

#define NODE1            "node01"  //node01|ambient|IR|difference
#define NODE2            "node02"  //node02|smokeADC|smokeV
//#define NODE3            'C'
//#define NODE4            'D'
//#define DELIMITER        ':'

XYPlot MyPlot("Sensors");
SoftwareSerial xbee(2, 3);

boolean rx_complete = false;

String inchar = "";
String IR = "";
String ambient = "";
String diff = "";
String smokeADC = "";
String smokeV = "";

uint8_t clearString = 0;

uint8_t n1_index[4]={0,0,0,0};
uint8_t n2_index[3]={0,0,0};

void setup() {

  Serial.begin(9600);
  xbee.begin(9600);

  delay(5000);

  inchar.reserve(30);
  IR.reserve(10);
  ambient.reserve(10);
  diff.reserve(10);
  smokeADC.reserve(10);
  smokeV.reserve(10);

  /*MyPlot.SetTitle("Fire-Hound Sensor Network");
  //MyPlot.SetXlabel("Cycle");
  //MyPlot.SetYlabel("Sensors");

  MyPlot.SetSeriesProperties("Ambient", Plot::Magenta, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("IR", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
  MyPlot.SetSeriesProperties("Smoke_ADC", Plot::Black, Plot::Solid, 2, Plot::Triangle);
  MyPlot.SetSeriesProperties("Smoke_V", Plot::Blue, Plot::Solid, 2, Plot::Circle);
  */
  //MyPlot.SetSeriesProperties("NODE_1", Plot::Magenta, Plot::Solid, 2, Plot::Square);
  //MyPlot.SetSeriesProperties("NODE_2", Plot::Red, Plot::Dashed, 2, Plot::Diamond);
  //MyPlot.SetSeriesProperties("NODE_3", Plot::Black, Plot::Solid, 2, Plot::Triangle);
  //MyPlot.SetSeriesProperties("NODE_4", Plot::Blue, Plot::Solid, 2, Plot::Circle);

}

void loop(void) {

  checkRx();

}

void checkRx(void) {

  if (xbee.available()) {

    char inByte = (char)xbee.read();

    if (inByte == '\r' || inByte == '\n') {

      rx_complete = true;
    }

    if (!rx_complete) {
      inchar += inByte;
    }
    if (rx_complete) {

      rx_complete = false;
      //Serial.print(inchar);
      //Serial.println();
      if (inchar.startsWith(NODE1)) {
        clearString = 1;
        
        /* String scanning */
        n1_index[0] = inchar.indexOf('|');
        n1_index[1] = inchar.indexOf('|', n1_index[0] + 1);
        n1_index[2] = inchar.indexOf('|', n1_index[1] + 1);
        n1_index[3] = inchar.length();
        
        /* String Seperation  */
        IR = inchar.substring(n1_index[0] + 1, n1_index[1]); 
        ambient = inchar.substring(n1_index[1] + 1, n1_index[2]);
        diff = inchar.substring(n1_index[2] + 1, n1_index[3]);
        
        /* Output to Serial */
        Serial.println("IR= " + IR);
        Serial.println("Ambient Temp= " + ambient);
        Serial.println("Diff Temp= " + diff);
        
        /* Output to MeguUnoLink */
        
        
        /* Old Code */ 
        //int sep1 = inchar.indexOf('|');
        //int sep2 = inchar.indexOf('|', sep1 + 1);
        //int sep3 = inchar.indexOf('|', sep2 + 1);
        //int lastindex = inchar.length();
        //IR = inchar.substring(sep1 + 1, sep2);
        //Serial.println("IR= " + IR);
        //ambient = inchar.substring(sep2 + 1, sep3);
        //Serial.println("Ambient Temp= " + ambient);
        //diff = inchar.substring(sep3 + 1, lastindex);
        //Serial.println("Diff Temp= " + diff);

      }
      else if (inchar.startsWith(NODE2)) {
        clearString = 2;
        
        /* String scanning */
        n2_index[0] = inchar.indexOf('|');
        n2_index[1] = inchar.indexOf('|', n2_index[0] + 1);
        n2_index[2] = inchar.length();
        
        /* String Seperation  */
        smokeADC = inchar.substring(n2_index[0] + 1, n2_index[1]); 
        smokeV = inchar.substring(n2_index[1] + 1, n2_index[2]);
        
        /* Output to Serial */
        Serial.println("Smoke ADC= " + smokeADC);
        Serial.println("Smoke Voltage= " + smokeV);
        
        /* Output to MeguUnoLink */
        
        
        /* Old Code */
        /*
        int sep1 = inchar.indexOf('|');
        int sep2 = inchar.indexOf('|', sep1 + 1);
        int lastindex = inchar.length();
        smokeADC = inchar.substring(sep1 + 1, sep2);
        Serial.println("smokeADC= " + smokeADC);
        smokeV = inchar.substring(sep2 + 1, lastindex);
        Serial.println("Ambient Temp= " + smokeV); */
      }
      inchar = "";
      switch (clearString) {
        case 1:
          IR = ambient = diff = "";
          break;
        case 2:
          smokeADC = smokeV = "";
          break;
        default:
          IR = ambient = diff = smokeADC = smokeV = "";
          break;
      }
      clearString = 0;
    }
  }
}







