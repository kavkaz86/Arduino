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

/* Series Properties */
#define Amb      "Ambient"
#define AmbProp  "m_2s3" //Plot::Magenta, Plot::Solid, 2, Plot::Square);
#define IRa       "IR"
#define IRProp  "r-2d3" //Plot::Red, Plot::Dashed, 2, Plot::Diamond);
#define Diff    "Diff"
#define DiffProp  "k_2^3"  //Plot::Black, Plot::Solid, 2, Plot::Triangle
#define SADC  "SmokeADC"
#define SADCProp  "b_2o3"  //Plot::Blue, Plot::Solid, 2, Plot::Circle);
#define SV  "SmokeV"
#define SVProp  "g_2x3"
#define pName  "FireHound Sensor Network"
#define p1Name  "Temperature vs Time (Node1)"
#define p1Y  "Temperature (Celsius)"
#define p2aName  "Smoke-Voltage vs Time (Node2)"
#define p2aY  "Smoke (V)"
#define p2bName  "Smoke-ADC vs Time (Node2)"
#define p2bY  "Analog Value"

TimePlot MyPlot("Sensors");
TimePlot Node1Plot("Node1-Temp");
TimePlot Node2Plot1("Smoke-V");
TimePlot Node2Plot2("Smoke-ADC");


SoftwareSerial xbee(2, 3);

boolean rx_complete = false;

String inchar = "";
String IR = "";
String ambient = "";
String diff = "";
String smokeADC = "";
String smokeV = "";

uint8_t clearString = 0;

uint8_t n1_index[4] = {0, 0, 0, 0};
uint8_t n2_index[3] = {0, 0, 0};

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

  MyPlot.SetSeriesProperties(Amb, AmbProp);
  MyPlot.SetSeriesProperties(IRa, IRProp);
  MyPlot.SetSeriesProperties(Diff, DiffProp);
  MyPlot.SetSeriesProperties(SADC, SADCProp);
  MyPlot.SetSeriesProperties(SV, SVProp);

  Node1Plot.SetTitle(p1Name);
  Node1Plot.SetYlabel(p1Y);

  Node2Plot1.SetTitle(p2aName);
  Node2Plot1.SetYlabel(p2aY);
  Node2Plot2.SetTitle(p2bName);
  Node2Plot2.SetYlabel(p2bY);

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
        Node1Plot.SendData(IRa, IR);
        Node1Plot.SendData(Amb, ambient);
        Node1Plot.SendData(Diff, diff);


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
        Node2Plot1.SendData(SV, smokeV);
        delay(20);
        Node2Plot2.SendData(SADC, smokeADC);

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

      inchar = ""; //clear string buffer.
      /* Clear Strings */
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
      clearString = 0; // reset flag.
    }
  }
}







