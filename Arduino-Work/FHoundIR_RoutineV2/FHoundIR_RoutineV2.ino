/* Author: Amro Qandour
 * S/W rev. : 1.0
 * Date: 07/07/15
 */

//#include  "Streaming.h"
#include "FireHound.h"
#include  "FireHoundComms.h"

/*/
 * Tx-Frame Types:
 * 1: Reporting frame
 * 2: Primary sensor detection frame
 * 3: Secondary sensor detection frame
 * 4: Diagnostics frame
 */
#define repF 1
#define irF  2
#define gasF 3
#define diagF  4

/*/
 * Others:
 */
//#define SENS_REQ "Hello"
#define GasSensor 1

#define zIR_vals  1
#define zIR_flag  2

#define rfSleep  1
#define rfWake   2

/*/
 * IR Sensor Pin Assignment:
 * a. IR Clock Pin: IR_clk_pins[Clk_1,Clk_2,Clk_3,Clk_4]
 * b. IR Data Pin: IR_data[data_1,data_2,data_3,data_4]
 * c. IR Active Pin: IR_act[act_1,act_2,act_3,act_4]
 */
static const byte IR_data[4] = {2, 5, 8, 11};
static const byte IR_clk[4]  = {3, 6, 9, 12};
static const byte IR_acq[4]  = {4, 7, 10, 13};
static const TempUnit SCALE = CELSIUS;

/*/
 * a. Dust Sensor Pin Assignment:
 * Dust_sensor[data, LED]
 *
 * b. Dust Sensor Time Thresholds:
 * Dust_TH[Sampling_time, Delta_time, Sleep_time]
 */
static const byte Dust_sensor[2] = {A0, 15};
static const long Dust_TH[3] = {280, 40, 9680};

/*/
 * a. Gas Sensor Pin Assignment:
 * Gas_sensor[data,EN]
 *
 * b. Gas Sensor Time Thresholds:
 * Gas_TH[Sampling_time, Delta_time]
 */
static const byte Gas_sensor[2] = {A2, 17};
static const long Gas_TH[2] = {30000, 1};
static const float gas_MAX = 3.8;

/*/
 * Battery Indicator Pin Assingment:
 * low_batt_ind[low_voltage_ind]
 */
static const byte low_batt_ind[1] = {18};

/*/
 * Communication Pin Assignment:
 * Comms[pin_sleep, baudrate]
 */
static const byte Comms_xbee[2] = {19, 1};

/*/
 * a. CO2 Sensor Pin Assignment:
 * CO2_init[pin_data, pin_alarm]
 *
 * b. CO2 Sensor Calibration parameters:
 * CO2_calib[init_Voltage, zero_point_voltage, reaction_voltage, DC_gain]
 *
 * c. CO2 Sensor Timing Thresholds:
 * CO2_TH[Sampling_time, Delta_time]
 */
static const byte CO2_init[2] = {A6, 21};
static const float CO2_calib[4] = {2.602, 0.324, 0.020, 8.5};
static const long CO2_TH[2] = {50, 5};

/*/
 * Data vars & Arrays:
 */
bool IR_flag[5] = {1, 0, 0, 0, 0};
bool c[4] = {0, 0, 0, 0};
bool  zIR[4] = {0, 0, 0, 0};
float IR_vals[4] = {0};
float gas_val[1] = {0};
int irmap = 0;
float DUST_vals[2] = {0}; 



/*/
 * Alarm flags:
 */
bool  IR_alarm = false;
bool  Gas_alarm = false;


/*/
 * Counters:
 */
int   gCount = 0;

/*/
 * Node Info:
 */
static const byte NODE_ID[1] = {'1'};

/*/
 * Class instances
 */
FHound IR_s;
FHound DST_s1;
FHound CO2_s1;
FHound Gas_s1;
FHComms comms;

void setup() {

  Serial.begin(9600);

  IR_setup(IR_clk, IR_data, IR_acq);
  //Comms_setup(Comms_xbee);
  //Dust_setup(Dust_sensor, Dust_TH);

}

void loop() {
  
  for (int i = 1; i < 5; i++) {
   
    IR_vals[i-1] = IR_s.getIRTemperature(i, CELSIUS);

  }
  for(int i=1 ; i < 5; i++){
    Serial.print(i); Serial.print(": "); Serial.println(IR_vals[i-1]);
  }
  IR_alarm = check_IR(IR_vals, 25);
  irmap = IR_s.IR_mapS(IR_flag);
  Serial.print("IR_Alarm: "); Serial.print(IR_alarm); Serial.print(" irmap: "); Serial.println(irmap);
  //Serial << "IR_Alarm: " << IR_alarm << " irmap: " << irmap << endl;
  cleanUp(); 
  
  /*while (!IR_alarm) {

    for (int i = 1; i < 5; i++) {
      
      IR_vals[i-1] = IR_s.getIRTemperature(i, CELSIUS);
      
    }

   // DUST_vals[0] = DST_s1.calc_Dust_Volt();
   // DUST_vals[1] = DST_s1.calc_Dust_Density(DUST_vals[0]); 
    
    IR_alarm = check_IR(IR_vals, 21);    
    if(gCount == 100){
      comms.sendDiag(NODE_ID[0]);
      gCount = 0;  
    }
    gCount++;

  }
  
  if (IR_alarm) {
    irmap = IR_s.IR_mapS(IR_flag);
    comms.sendTX(irF, NODE_ID[0], irmap, IR_vals, 4);
  }

  cleanUp(); */


}

void Dust_setup(const byte dust[], const long dust_th[]) {

  DST_s1.setDust(dust[0], dust[1]);
  DST_s1.setDustTh(dust_th[0], dust_th[1], dust_th[2]);

}

void Comms_setup(const byte _comms[]) {

  comms.setComms(_comms[0], _comms[1]);

}

void IR_setup(const byte clk[], const byte data[], const byte acq[]) {

  for (int i = 1; i < 5; i++) {

    IR_s.setIR(acq[i - 1], clk[i - 1], data[i - 1], i);

  }
}

/**
 * IR Sensor Detection Algorithm:
 */
bool check_IR(const float data[], float IR_MAX) {

  const float a = data[0];
  const float b = data[1];
  const float c = data[2];
  const float d = data[3];

  if ((a > IR_MAX) || (b > IR_MAX) || (c > IR_MAX) || (d > IR_MAX) ) {
    if (a > IR_MAX) {
      IR_flag[1] = true;
    }
    if (b > IR_MAX) {

      IR_flag[2] = true;
    }
    if (c > IR_MAX) {

      IR_flag[3] = true;
    }
    if (d > IR_MAX) {

      IR_flag[4] = true;
    }

    return true;

  }
  return false;
}

/**
 * Housekeeping and Maintenance:
 * a. Reset variables
 * b. Reset data arrays.
 */
void clearIR(int type, const bool IR[]) {

  if (type == zIR_flag) {
    for (int i = 1; i < 5; i++) {
      IR_flag[i] = IR[i];
    }
  }
  else if (type == zIR_vals) {
    for (int i = 0; i < 4; i++) {
      IR_vals[i] = IR[i];
    }
  }
}

void cleanUp() {

  Gas_alarm = false;
  IR_alarm = false;
  irmap = 0;
  clearIR(zIR_vals, zIR);
  clearIR(zIR_flag, zIR);

}
