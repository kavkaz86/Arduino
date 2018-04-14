#include "MegunoLink.h"
#include "Streaming.h"


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(10000);

  /*
   * Node 1
   */
  Serial.print(F("{TABLE:Node_1|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_1|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_1|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_1|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_1|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_1|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_1|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_1|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_1|Description|Dust_ppm|ppm}"));

  /*
   * Node 2
   */
  Serial.print(F("{TABLE:Node_2|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_2|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_2|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_2|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_2|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_2|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_2|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_2|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_2|Description|Dust_ppm|ppm}"));

  /*
   * Node 3
   */
  Serial.print(F("{TABLE:Node_3|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_3|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_3|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_3|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_3|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_3|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_3|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_3|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_3|Description|Dust_ppm|ppm}"));

  /*
   * Node 4
   */
  Serial.print(F("{TABLE:Node_4|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_4|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_4|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_4|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_4|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_4|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_4|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_4|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_4|Description|Dust_ppm|ppm}"));

  /*
   * Node 5
   */
  Serial.print(F("{TABLE:Node_5|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_5|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_5|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_5|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_5|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_5|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_5|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_5|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_5|Description|Dust_ppm|ppm}"));

  /*
   * Node 6
   */
  Serial.print(F("{TABLE:Node_6|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_6|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_6|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_6|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_6|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_6|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_6|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_6|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_6|Description|Dust_ppm|ppm}"));


  /*
   * Node 7
   */
  Serial.print(F("{TABLE:Node_7|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_7|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_7|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_7|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_7|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_7|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_7|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_7|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_7|Description|Dust_ppm|ppm}"));

  /*
   * Node 8
   */
  Serial.print(F("{TABLE:Node_8|Description|NODE_ID|}"));
  Serial.print(F("{TABLE:Node_8|Description|FRAME_ID|}"));
  Serial.print(F("{TABLE:Node_8|Description|NODE_LOCATION|Degrees}"));
  Serial.print(F("{TABLE:Node_8|Description|IR_1|Celsius}"));
  Serial.print(F("{TABLE:Node_8|Description|IR_2|Celsius}"));
  Serial.print(F("{TABLE:Node_8|Description|IR_3|Celsius}"));
  Serial.print(F("{TABLE:Node_8|Description|IR_4|Celsius}"));
  Serial.print(F("{TABLE:Node_8|Description|Dust_V|Voltage}"));
  Serial.print(F("{TABLE:Node_8|Description|Dust_ppm|ppm}"));
  
  
  /*
   * Constants
   */
  Serial.print(F("{MAP|SET|NODE_1|-31.749641|115.771035}"));
  Serial.print(F("{TABLE:Node_1|SET|NODE_ID|0013A20021759210}"));
  Serial.print(F("{TABLE:Node_1|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_1|SET|NODE_LOCATION|-31.749641, 115.771035}"));

  Serial.print(F("{MAP|SET|NODE_2|-31.750408|115.769383}"));
  Serial.print(F("{TABLE:Node_2|SET|NODE_ID|0013A20061752410}"));
  Serial.print(F("{TABLE:Node_2|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_2|SET|NODE_LOCATION|-31.750408, 115.769383}"));

  Serial.print(F("{MAP|SET|NODE_3|-31.751192|115.770864}"));
  Serial.print(F("{TABLE:Node_3|SET|NODE_ID|0013A20061354226}"));
  Serial.print(F("{TABLE:Node_3|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_3|SET|NODE_LOCATION|-31.751192, 115.770864}"));

  Serial.print(F("{MAP|SET|NODE_4|-31.748355|115.773471}"));
  Serial.print(F("{TABLE:Node_4|SET|NODE_ID|0013A20061752410}"));
  Serial.print(F("{TABLE:Node_4|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_4|SET|NODE_LOCATION|-31.748355, 115.773471}"));

  Serial.print(F("{MAP|SET|NODE_5|-31.750350|115.772849}"));
  Serial.print(F("{TABLE:Node_5|SET|NODE_ID|0013A20061888410}"));
  Serial.print(F("{TABLE:Node_5|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_5|SET|NODE_LOCATION|-31.750350,115.772849}"));

  Serial.print(F("{MAP|SET|NODE_6|-31.751299|115.772624}")); //-31.751299, 115.772624
  Serial.print(F("{TABLE:Node_6|SET|NODE_ID|0013A2006177710}"));
  Serial.print(F("{TABLE:Node_6|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_6|SET|NODE_LOCATION|-31.751299, 115.772624}"));

  Serial.print(F("{MAP|SET|NODE_7|-31.753616|115.771412}")); //-31.753616, 115.771412
  Serial.print(F("{TABLE:Node_7|SET|NODE_ID|0013A20061752555}"));
  Serial.print(F("{TABLE:Node_7|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_7|SET|NODE_LOCATION|-31.753616, 115.771412}"));

  Serial.print(F("{MAP|SET|NODE_8|-31.753497|115.770060}")); //-31.753497, 115.770060
  Serial.print(F("{TABLE:Node_8|SET|NODE_ID|0013A20061756560}"));
  Serial.print(F("{TABLE:Node_8|SET|FRAME_ID|1}"));
  Serial.print(F("{TABLE:Node_8|SET|NODE_LOCATION|-31.753497, 115.770060}"));

}

int n=0; 

void loop() {
  // put your main code here, to run repeatedly:

  float IR1, IR2, IR3, IR4, Dustv, Dustppm, IRAVG = 0; 
  /*
   * Node 1
   */
  IR1 = n;
  IR2 = 2.0*n;
  IR3 = 3.0*n;
  IR4 = 3.0*n-1;
  Dustv = 2.3*n ;
  Dustppm = 2.6*n/4;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 
  Serial << F("{TABLE:Node_1|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_1|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_1|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_1|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_1|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_1|SET|Dust_ppm|") << Dustppm << "}" << endl; 
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_1:r-s4|T|") << IRAVG << "}" << endl;

  
  /*
   * Node 2
   */
  IR1 = 1.0*n;
  IR2 = 2.3*n;
  IR3 = 3.2*n;
  IR4 = 3.5*n-1;
  Dustv = 4.1*n/2.0 ;
  Dustppm = 2.2*n/3.0;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 

  Serial << F("{TABLE:Node_2|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_2|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_2|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_2|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_2|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_2|SET|Dust_ppm|") << Dustppm << "}" << endl;
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_2:b-s4|T|") << IRAVG << "}" << endl;
  
  /*
   * Node 3
   */
  IR1 = 1.4*n;
  IR2 = 2.53*n;
  IR3 = 3.26*n;
  IR4 = 3.32*n-1;
  Dustv = 4.21*n/2.0 ;
  Dustppm = 2.6*n/3.32;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 
  Serial << F("{TABLE:Node_3|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_3|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_3|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_3|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_3|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_3|SET|Dust_ppm|") << Dustppm << "}" << endl;
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_3:k-s4|T|") << IRAVG << "}" << endl;

  /*
   * Node 4
   */
  IR1 = 2.4*n;
  IR2 = 2.66*n;
  IR3 = 2.26*n;
  IR4 = 3.35*n-1;
  Dustv = 1.2*n ;
  Dustppm = Dustv/2.4;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 
  
  Serial << F("{TABLE:Node_4|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_4|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_4|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_4|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_4|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_4|SET|Dust_ppm|") << Dustppm << "}" << endl;
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_4:g-s4|T|") << IRAVG << "}" << endl;

  /*
   * Node 5
   */
  IR1 = 2.24*n;
  IR2 = 2.86*n;
  IR3 = 2.36*n;
  IR4 = 3.75*n-1;
  Dustv = 1.26*n ;
  Dustppm = Dustv/2.4;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 
  
  Serial << F("{TABLE:Node_5|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_5|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_5|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_5|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_5|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_5|SET|Dust_ppm|") << Dustppm << "}" << endl;
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_5:g-s4|T|") << IRAVG << "}" << endl;

  /*
   * Node 6
   */
  IR1 = 2.423*n;
  IR2 = 2.536*n;
  IR3 = 2.126*n;
  IR4 = 3.655*n-1;
  Dustv = 1.122*n ;
  Dustppm = Dustv/2.4;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 
  
  Serial << F("{TABLE:Node_6|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_6|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_6|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_6|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_6|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_6|SET|Dust_ppm|") << Dustppm << "}" << endl;
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_6:g-s4|T|") << IRAVG << "}" << endl;

  /*
   * Node 7
   */
  IR1 = 2.76*n;
  IR2 = 2.32*n;
  IR3 = 2.64*n;
  IR4 = 3.231*n-1;
  Dustv = 1.77*n ;
  Dustppm = Dustv/2.4;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 
  
  Serial << F("{TABLE:Node_7|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_7|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_7|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_7|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_7|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_7|SET|Dust_ppm|") << Dustppm << "}" << endl;
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_7:g-s4|T|") << IRAVG << "}" << endl;

  /*
   * Node 8
   */
  IR1 = 2.655*n;
  IR2 = 2.232*n;
  IR3 = 2.633*n;
  IR4 = 3.654*n-1;
  Dustv = 1.775*n ;
  Dustppm = Dustv/2.4;
  IRAVG = (IR1 + IR2 + IR3 + IR4)/4 ; 
  
  Serial << F("{TABLE:Node_8|SET|IR_1|") << IR1 << "}" << endl;
  Serial << F("{TABLE:Node_8|SET|IR_2|") << IR2 << "}" << endl;
  Serial << F("{TABLE:Node_8|SET|IR_3|") << IR3 << "}" << endl;
  Serial << F("{TABLE:Node_8|SET|IR_4|") << IR4 << "}" << endl;
  Serial << F("{TABLE:Node_8|SET|Dust_V|") << Dustv << "}" << endl;
  Serial << F("{TABLE:Node_8|SET|Dust_ppm|") << Dustppm << "}" << endl;
  Serial << F("{TIMEPLOT:IR_ALL|DATA|NODE_8:g-s4|T|") << IRAVG << "}" << endl;
  
  n++;
  delay(1000);

}
