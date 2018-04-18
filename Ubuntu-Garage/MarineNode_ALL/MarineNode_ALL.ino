/*
The E.C. Circuit will respond:
EC,TDS,SAL<CR>
Where:
EC is the electrical conductivity in μs/cm
TDS is Total Dissolved Solids (referenced to Kcl)
SAL is Salinity (Practical Salinity scale 1978) expressed as a whole number only.
*/


/*
**********************************************************************************************
 *                INCLUDE FILES
 **********************************************************************************************
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>                                                  
#include <OneWire.h>
#include "Streaming.h"
#include <XBee.h>

/*
**********************************************************************************************
 *                LOCAL DEFINES
 **********************************************************************************************
 */

#define	sleep_pin  8
XBee xbee = XBee();
OneWire  ds(4);

String inputstring = "";         
String oxygen = "";
String ph="";
String sal="";
String tx = "";
boolean input_stringcomplete = false;                                          
boolean sensor_stringcomplete = false;
boolean	sleep_en = false;                                          
boolean oxy_setup = false; 
float celsius, fahrenheit;
byte addr[8];
uint8_t temp_counter, cont_count=0;

XBeeAddress64  co = XBeeAddress64(0x00000000, 0x00000000);  
//XBeeAddress16  Coordinator = XBeeAddress16(0x1111);

/*
**********************************************************************************************
 *                EXTERNAL INTERRUPTS
 **********************************************************************************************
 */



/*
**********************************************************************************************
 *                DIGITAL OUTPUTS
 **********************************************************************************************
 */


/*
**********************************************************************************************
 *                GLOBAL VARIABLE DEFINITIONS
 **********************************************************************************************
 */

volatile uint8_t sensorcount = 0;  // Sensor ID 
/*
**********************************************************************************************
 *                EXTERNAL INTERUPT ROUTINES
 **********************************************************************************************
 */


/*  WDT TIMER          */
ISR(WDT_vect)
{
  sensorcount++;  
}

/*
**********************************************************************************************
 **********************************************************************************************
 *                Temperature Functions
 **********************************************************************************************
 **********************************************************************************************
 */

void tempinit(){

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return;
  }

}

float tempread(){
  float temp=0; 
  byte data[12];

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( byte i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];

  byte cfg = (data[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time

  temp = (float)raw / 16.0;

  return temp; 
}


/*
**********************************************************************************************
 **********************************************************************************************
 *                INITIALISATION FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void setup()
{
  pinMode(sleep_pin, OUTPUT);   
  // Config serial baud-rate 
  Serial.begin(115200); //debug port
  xbee.setSerial(Serial);
  Serial1.begin(38400); // Oxy
  Serial2.begin(38400); // pH
  Serial3.begin(38400); // Sal

  tempinit();

  oxygen.reserve(30);
  ph.reserve(30);
  sal.reserve(30);
  tx.reserve(100);
  
  delay(5000);


  // Salinity chip initialisation

  //Oxygen chip initialisation 
  // tx-> %, tx->r, rx<-oxy_string. if one of oxy_string_chars == % -> exit
  // else tx->%. 
  // ensures oxy_string in loop() consists of -> mgL, %oxy

  //Serial1 << "%" << '\r'; delay(1000);  //debug                                                                                                      
  
  oxy_init(); 
   
  
  ph_init();
  
 
  sal_init(); 


}


/**********************************************************
//////////////  Oxyygen Sensor Init Function \\\\\\\\\\\\\\\
************************************************************/
void oxy_init(){
  
  Serial1 << "e" << '\r';
  delay(1000);
  
  Serial1 << "r" << '\r'; 
  delay(1000); 


  while(Serial1.available()){                                                                 
    char inchar = (char)Serial1.read();                            
    //Serial << inchar ; //debug

    if(inchar == ','){
      oxy_setup = true;
    }              
    if(inchar == '\r') {
      sensor_stringcomplete = true;
    }
  }
  if(sensor_stringcomplete){
    Serial << endl;
    if(!oxy_setup){
      Serial1 << "%" << '\r'; 
      delay(1000);
      Serial << "M-Oxy Initialiazed" << endl;
    }
    else{
      Serial<< "Oxy Initialiazed" << endl;
      sensor_stringcomplete=false;
    } 
  }
  delay(1000);
}

/*****************************************************
//////////////  pH Sensor Init Function \\\\\\\\\\\\\\\
******************************************************/
void ph_init(){
  
  Serial2 << "e" << '\r';
  Serial << "pH Sensor Initialised" << endl; 
  delay(1000);
}

/************************************************************
//////////////  Salinity Sensor Init Function \\\\\\\\\\\\\\\
*************************************************************/
void sal_init(){
  
  Serial3 << "e" << '\r';
  Serial << "Salinity Sensor Initialised" << endl;
  delay(1000);
}



void loop(){
  
  // Sleep for 2 mins
  while(sensorcount<5){
  wtdsleep();
  delay(200);    
  }

  // Node wakes up after two minutes
  //digitalWrite(sleep_pin, LOW); // Disable radio sleep
  //delay(15000); // 15-seconds required for radio reassociation 

  //celsius=tempread(); // Read temperature 

  /***************Salinity Function*************** 
   * // Take contineous readings temp-comps. for 25 seconds = 25 frames.
   * // When frames = 25 -> Save that as the final output string
   * // Salinity output frame = #TDS,SAL,TIG#
   */
   
  sal_read();

  /***************Oxygen Function*****************
   * // Take contineous temp-comps. readings 5 seconds = 10 frames.
   * // When frames = 10 -> Save that as the final output string
   * // Oxygen output frame = #mg/L,Oxy_%#
   */
  
   oxy_read(); 

  /**************pH Function********************* 
   * // Take single temp-comps. readings
   * // Read final output 
   * // pH output frame = #pH_IND#
   */
  
   ph_read();
  
  /**************Tx Function*********************
   * // Tx_Format = [Salinity_Frame#Oxygen_Frame#pH_Frame
   * //           =  #Microsiemens (μs/cm),TDS(in PPM),Salinity(PSS-78)#Oxy_%,mg/L#pH_IND
   */
  Serial << "#" << sal << "#" << oxygen << "#" << ph << "#" << endl; 
  delay(1000);
  tx = sal + oxygen + ph + '\r'; 
  char str[100];
  tx.toCharArray(str, 128); 
  Tx64Request tx = Tx64Request(Coordinator, (uint8_t *)str, sizeof(str));
  xbee.send(tx);
  
  /*
  /////////////// RESET \\\\\\\\\\\\\\\
  */
  sleep_en = false; // Reset sleep_enable for next sleeping period
  sensorcount = 0; // Reset sensor_count used for sleep func.
  oxygen = "";
  ph = "";
  sal = "";  

}

void oxy_read(){
  
  //myserial.print(celsius); delay(100); myserial.print(",s"); delay(100); myserial.print('\r'); delay(100);
  Serial1 << "22,s" << '\r'; delay(1000);
  // Enable continous mode for the oxygen chip
  //myserial.print("c"); delay(100); myserial.print('\r'); delay(100);
  Serial1 << "c" << '\r'; delay(1000);   

  // Take contineous reading for 5 seconds    
  while(cont_count < 10){
    // Read software serial port for incoming oxygen data
    while (Serial1.available()) {
      char inchar = (char)Serial1.read();
      if(cont_count ==9){
      
       oxygen+=inchar;
      }
       // Transmit data when <CR> is received           
       if (inchar == '\r') { 
                             if(cont_count == 9){
                               Serial << "O:" << oxygen << endl; 
                               //oxygen = "";
                             }
                             cont_count++;  
                           }  
    }
  }
  // Disable continous mode for the oxygen chip
  //myserial.print("e"); delay(100); myserial.print('\r'); delay(100);
  Serial1 << "e" << '\r'; delay(1000);
  cont_count=0;
  delay(10000);
}

void ph_read(){
  
  //myserial.print(celsius); delay(100); myserial.print('\r'); delay(100);
  Serial2 << "22" << '\r'; delay(1000);
  
  // Take a single pH reading
  //myserial.print("r"); delay(100); myserial.print('\r'); delay(200);
  Serial2 << "r" << '\r'; delay(1000);
  
  // Read software serial port for incoming pH data
  while (Serial2.available()) {
    char inchar = (char)Serial2.read();
    ph+=inchar;
    // Transmit data when <CR> is received           
    if (inchar == '\r') { 
      Serial << "PH:" << ph << endl; 
      delay(100);                               
      //ph = "";  
    }  
  }
}

void sal_read(){
  
    // Enable continous mode for the salinity chip
  //myserial.print(celsius); delay(100); myserial.print(",c"); delay(100); myserial.print('\r'); delay(100);
  Serial3 << "22,c" << '\r'; delay(1000);   
  // Take contineous reading for 5 seconds    
  while(cont_count < 25){
    // Read software serial port for incoming salinity data
    while (Serial3.available()) {
      char inchar = (char)Serial3.read();
      
      if(cont_count == 24){
        sal+=inchar;
      }
      // Transmit data when <CR> is received           
      if (inchar == '\r') {
        if(cont_count == 24){ 
          Serial << "S:" << sal << endl;                                
          //sal = "";
        }
        cont_count++;
      }  
    }
  }
  // Disable continous mode for the oxygen chip
  //myserial.print("e"); delay(100); myserial.print('\r'); delay(100);
  Serial3 << "e" << '\r'; 
  cont_count=0;
  delay(5000); 
}


/*
**********************************************************************************************
 **********************************************************************************************
 *                SLEEP FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void wtdsleep(){

  // Enabling radio sleep mode 
  if(!sleep_en){   
    digitalWrite(sleep_pin, HIGH);
    sleep_en = true; 
  }

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  MCUSR &= ~(1<<WDRF);

  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */

  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  /* Now enter sleep mode. */
  sleep_mode();

  /* The program will continue from here after the WDT timeout*/

  sleep_disable(); /* First thing to do is disable sleep. */

  /* Re-enable the peripherals. */
  power_all_enable();

  wdt_reset();
  MCUSR = 0;
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;

}






