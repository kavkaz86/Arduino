/*
**********************************************************************************************
 *                INCLUDE FILES
 **********************************************************************************************
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <OneWire.h>

/*
**********************************************************************************************
 *                LOCAL DEFINES
 **********************************************************************************************
 */

/*  SENSOR UNITS           */
#define  UVSensor    1
#define  PARSensor   2
#define  TURSensor   3 


/*
**********************************************************************************************
 *                EXTERNAL VARIABLES
 **********************************************************************************************
 */

/*  INPUT PINS            */
const int UV_PIN = A0;  //UV SENSOR ANALOG PIN = A0  
const int PAR_PIN =A1;  //PAR SENSOR ANALOG PIN = A1 
const int TUR_PIN = A2; //TURBIDITY SENSOR ANALOG PIN = A2 
const int TEMP_PIN = 9; //TEMPERATURE SENSOR DIGITAL PIN = 9  

/*  VARIABLE DEFINITIONS  */
float UV_SENSOR = 0; 
float PAR_SENSOR = 0; 
float TUR_SENSOR = 0; 
int   TEMP_SENSOR = 0; 

byte i;
byte present = 0;
byte data[12];
byte addr[8];
//char sensorstring[10]; 
/*
**********************************************************************************************
 *                EXTERNAL INTERUPT ROUTINES
 **********************************************************************************************
 */

/*  WDT TIMER          */
ISR(WDT_vect)
{
} 

OneWire  ds(TEMP_PIN);



/*
**********************************************************************************************
 *                TEMPERATURE SENSOR DS18B20 INITIALISATION
 **********************************************************************************************
 */
 
void DS18B20_INIT(void){
  
  if ( !ds.search(addr)) {
      delay(1000);
      ds.reset_search();
      return;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      delay(1000);
      return;
  }
}
  
  
void setup(){


  pinMode(UV_PIN, INPUT);
  pinMode(PAR_PIN, INPUT);
  pinMode(TUR_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  analogReference(INTERNAL); 
  Serial.begin(9600);  
  DS18B20_INIT();

}

/*
**********************************************************************************************
 **********************************************************************************************
 *                SLEEP FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

void sleepNow()         // here we put the arduino to sleep
{

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

/*
**********************************************************************************************
 **********************************************************************************************
 *                UV + PAR SENSOR FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

float getUVPAR(int pin){
  
  int rawSens = 0; 
  float milliVolts = 0; 
  float sensor = 0;
  
  rawSens = analogRead(pin);
  milliVolts = ((float(rawSens) * 1100)/1023);
  sensor = milliVolts * 5;
  
  return(sensor);
}

/*
**********************************************************************************************
 **********************************************************************************************
 *                TURBIDITY SENSOR FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */
 
float getTUR(int pin){
  
  int rawSens = 0;
  float turVolts = 0;
  
  rawSens = analogRead(pin);
  turVolts = ((float(rawSens) * 1100)/1023);
  
  return(turVolts); 
}

/*
**********************************************************************************************
 **********************************************************************************************
 *                TEMPERATURE SENSOR FUNCTION
 **********************************************************************************************
 **********************************************************************************************
 */

int getTEMP(void) {
  int HighByte, LowByte, SignBit, Whole, Fract, TReading, Tc_100;
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  
  if (SignBit) {
    TReading = -TReading;
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  Whole = Tc_100 / 100;          // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
  if (Fract > 49) {
    if (SignBit) {
      --Whole;
    } else {
      ++Whole;
    }
  }
  
  return(Whole);
}

/*
**********************************************************************************************
 **********************************************************************************************
 *                APPLICATION PROGRAM
 **********************************************************************************************
 **********************************************************************************************
 */

void loop(){
  //boolean clear_sensor_array = false;
  /*
  if(clear_array() == true){
    UV_SENSOR = getUVPAR(UV_PIN);
    format_tx(UVSensor, UV_SENSOR);
    Serial.print(sensorstring); 
    delay(1000);
    if(clear_array() == true){
      PAR_SENSOR = getUVPAR(PAR_PIN);
      format_tx(PARSensor, PAR_SENSOR);
      Serial.print(sensorstring);
      delay(1000);
      if(clear_array() == true){
        TUR_SENSOR = getTUR(TUR_PIN);
        format_tx(TURSensor, TUR_SENSOR);
        Serial.print(sensorstring);
        delay(1000);
      }
    }
  } */
  char sensorstring[10];
  UV_SENSOR = getUVPAR(UV_PIN);
  //Serial.print(UV_SENSOR);
  snprintf(sensorstring, sizeof(sensorstring), "%f", 3);
  Serial.print(sensorstring);
  //clear_array();
  //RESET VARIABLES
  //UV_SENSOR = 0;
  //PAR_SENSOR = 0; 
  //TUR_SENSOR = 0; 
  //TEMP_SENSOR = 0;
  delay(1000);
}
/*

char format_tx(uint8_t sensor_ID, float sensor_value){
  
  switch(sensor_ID){
    
    case  UVSensor:
          sprintf(sensorstring, "C:%f %c", UV_SENSOR, '\r');
          break;

    case  PARSensor:
          sprintf(sensorstring, "D:%f %c", PAR_SENSOR, '\r');
          break;
          
    case  TURSensor:
          sprintf(sensorstring, "H:%f %c", TUR_SENSOR, '\r');
          break; 
       
    case  TEMPSensor:
          sprintf(sensorstring, "A:%f %c", TEMP_SENSOR, '\r');
          break;
    
    default:  break;

  }
}

boolean  clear_array(void){
  //boolean  clear_tx_frame = false; 
  for(int i=0; i++; i<9){
    
    sensorstring[i]='0'; 
  }
  
  if(sizeOf(sensorstring) == 0){ clear_tx_frame = true;) 
  else{
    clear_tx_frame = false;
  } 
  return(1); 
} */

