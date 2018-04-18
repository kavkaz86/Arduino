/* Author: Amro Qandour
 * S/W rev. : 1.0
 * Date: 07/07/15
 */

#include "FireHound.h"
#include  "FireHoundComms.h"

/*/
 * Tx-Frame Types:
 * 1: Reporting frame
 * 2: Primary sensor detection frame
 * 3: Secondary sensor detection frame
 * 4: Diagnostics frame
 */
#define repF  1
#define irF   2
#define gasF  3
#define diagF 4

/*/
 * Others:
 */
#define CO2_ID 1
#define CH4_ID 2
#define LPG_ID 3
#define CO_ID  4
#define AD_ID  5
#define ALL_SENS  6

#define zIR_vals    1
#define zIR_flag    2
#define zRep_flag   3
#define zDust_flag  4
#define zGas_flag   5

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
static const byte Dust_sensor[2] = {15, A0};
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
//static const float gas_MAX = 3.8;

/*/
 * Battery Indicator Pin Assingment:
 * low_batt_ind[low_voltage_ind]
 */
static const byte low_batt_ind[1] = {19};

/*/
 * Communication Pin Assignment:
 * Comms[pin_sleep, baudrate]
 */
static const byte Comms_xbee = 18;

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
static const byte CO2_init[2] = {A2, 17};
static const float CO2_calib[4] = {2.602, 0.324, 0.020, 8.5};
static const long CO2_TH[2] = {50, 5};

/*/
 * Data vars & Arrays:
 */
int irmap = 0;
bool IR_flag[5] = {1, 0, 0, 0, 0};
bool c[4] = {0, 0, 0, 0};
float IR_vals[4] = {0};
float DUST_vals[2] = {0};
float REP_vals[8] = {0};
float CO2_vals[2] = {0};




/*/
 * Alarm flags:
 */
bool  IR_alarm = false;
bool  Gas_alarm = false;
bool  rep_flag = false;
bool  diag_flag = false;
bool  AD_alarm = false;
bool  repRTN = false;
bool  diagRTN = false;


/*/
 * Counters:
 */
int   gCount = 0;
int   repCount = 0;
int   diagCount = 0;

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
  
  IR_setup(IR_clk, IR_data, IR_acq);
  Comms_setup(Comms_xbee);
  Dust_setup(Dust_sensor, Dust_TH);
  CO2_setup(CO2_init, CO2_calib, CO2_TH);
  //pinMode(18, OUTPUT);
  //digitalWrite(18, HIGH);
  //delay(100);


}

void loop() {

  while (!IR_alarm && !rep_flag && !diag_flag && !AD_alarm && !Gas_alarm) {

    for (int i = 1; i < 5; i++) {
      
      IR_vals[i - 1] = IR_s.getIRTemperature(i, CELSIUS);
    }

    IR_alarm = check_IR(IR_vals, 28);

    if (!IR_alarm) {

      DUST_vals[0] = DST_s1.calc_Dust_Volt();
      DUST_vals[1] = DST_s1.calc_Dust_Density(DUST_vals[0]);
      AD_alarm = check_airDensity(DUST_vals, 0.5);

      if (!AD_alarm) {

        CO2_vals[0] = CO2_s1.calc_CO2_v();
        CO2_vals[1] = CO2_s1.calc_CO2_percent(CO2_vals[0]);
        Gas_alarm = check_gasPPM(CO2_vals, 400);

        if (!Gas_alarm) {

          if (repCount == 10) { //&& ! IR_alarm

            rep_flag = true;
            repRTN = true;

            for (int i = 0 ; i < 4; i++) {
              REP_vals[i] = IR_vals[i];
            }
            for (int i = 4; i < 6 ; i++) {
              REP_vals[i] = DUST_vals[i - 4];
            }
            for (int i = 6; i < 8 ; i++) {
              REP_vals[i] = CO2_vals[i - 6];
            }
          }
          if (diagCount == 50) {
            diag_flag = true;
            diagRTN = true;
          }
        }
      }
    }
    diagCount++;
    repCount++;
    delay(50);

  }
  
  //comms.RF_mode(rfWake);
  //delay(50);
  
  if (IR_alarm) {
    irmap = IR_s.IR_mapS(IR_flag);
    while(!comms.sendTX(irF, NODE_ID[0], irmap, IR_vals, 4)){
      delay(200);
    }
    IR_alarm = false;
    irmap = 0;
    clearVals(zIR_vals);
    clearVals(zIR_flag);
  }
  else if (AD_alarm) {
    while(!comms.sendTX(gasF, NODE_ID[0], AD_ID, DUST_vals, 2)){
      delay(200);
    }
    AD_alarm = false;
    clearVals(zDust_flag);
  }
  else if (Gas_alarm) {
    while(!comms.sendTX(gasF, NODE_ID[0], CO2_ID, CO2_vals, 2)){
      delay(200);
    }
    Gas_alarm = false;
    clearVals(zGas_flag);
  }
  else {
    if (rep_flag) {
      repCount = 0;
      while(!comms.sendTX(repF, NODE_ID[0], ALL_SENS, REP_vals, 8)){
        delay(200);
      }
      rep_flag = false;
      clearVals(zRep_flag);
      repRTN = false;
    }

    if (diag_flag) {
      diagCount = 0;
      comms.sendDiag(NODE_ID[0]);
      diag_flag = false;
      diagRTN = false;
    }
  }
  error_ctrl();
  
  //comms.RF_mode(rfSleep);
}

void Dust_setup(const byte dust[], const long dust_th[]) {

  DST_s1.setDust(dust[0], dust[1]);
  DST_s1.setDustTh(dust_th[0], dust_th[1], dust_th[2]);

}

void Comms_setup(const byte _comms) {

  comms.setComms(_comms, 38400);

}

void IR_setup(const byte clk[], const byte data[], const byte acq[]) {

  for (int i = 1; i < 5; i++) {

    IR_s.setIR(acq[i - 1], clk[i - 1], data[i - 1], i);

  }
}

void CO2_setup(const byte initCO2[], const float calib[], const long th[]) {

  CO2_s1.setCO2(initCO2[0], initCO2[1], calib[3]);
  CO2_s1.setCO2_th(th[0], th[1]);
  CO2_s1.setCO2_Calib(calib[0], calib[1], calib[2]);
  CO2_s1.setGasHeater(true, 1);

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

bool check_airDensity(float _dustData[], float DUST_max) {
  if (_dustData[1] > DUST_max) {
    return true;
  }
  return false;
}

bool  check_gasPPM(float _gasData[], int GAS_max) {
  if (_gasData[1] > GAS_max) {
    return true;
  }
  return false;
}

/**
 * Housekeeping and Maintenance:
 * a. Reset variables
 * b. Reset data arrays.
 */
void clearVals(int type) {

  if (type == zIR_flag) {
    for (int i = 1; i < 5; i++) {
      IR_flag[i] = '\0';
    }
  }
  else if (type == zIR_vals) {
    for (int i = 0; i < 4; i++) {
      IR_vals[i] = '\0';
    }
  }
  else if (type == zRep_flag) {
    for (int i = 0; i < 8; i++) {
      REP_vals[i] = '\0';
    }
  }
  else if (type == zDust_flag) {
    for (int i = 0; i < 2; i++) {
      DUST_vals[i] = '\0';
    }
  }
  else if (type == zGas_flag) {
    for (int i = 0; i < 2; i++) {
      CO2_vals[i] = '\0';
    }
  }
}

void error_ctrl() {
  /*
  if(IR_alarm){
    IR_alarm = false; 
  }
  else if(AD_alarm){
    AD_alarm = false; 
  }
  else if(Gas_alarm){
    Gas_alarm = false;
  }
  else if(repRTN){
    repCount = 0;
    rep_flag = false;   
  }
  else if(diagRTN){
    diagCount = 0;
    diag_flag = false;
  } */
  if(repRTN || repCount > 10){
    repCount = 0;
    rep_flag = false;   
  }
  if(diagRTN || diagCount > 50){
    diagCount = 0;
    diag_flag = false;
  }
  
  IR_alarm = false; 
  AD_alarm = false; 
  Gas_alarm = false;
  //repCount = 0;
  //rep_flag = false;
  //diagCount = 0;
  //diag_flag = false;   
}
