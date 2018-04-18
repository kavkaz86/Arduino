/*/
 * Bushfire Monitoring System S/W for Remote Sensing Nodes
 * Author: Amro Qandour
 * S/W rev. : 1.0
 * Date: 07/07/15
 */

#include  "Streaming.h"
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


/*/
 * System configuration loop
 */
void setup() {

  IR_setup(IR_clk, IR_data, IR_acq);

  Dust_setup(Dust_sensor, Dust_TH);

  Gas_setup(Gas_sensor, Gas_TH);

  CO2_setup(CO2_init, CO2_calib, CO2_TH);

  Comms_setup(Comms_xbee);

}

/*/
 * Application loop
 */
void loop() {
  comms.RF_mode(rfSleep);
  while (!IR_alarm || !Gas_alarm) {

    for (int i = 1; i < 5; i++) {

      IR_vals[i-1] = IR_s.getIRTemperature(i, CELSIUS);

    }

    IR_alarm = check_IR(IR_vals, 50);

    if (!IR_alarm && gCount == 100) {

      Gas_s1.setGasHeater(true);
    }

    if (!IR_alarm && gCount == 150) {

      gas_val[0] = Gas_s1.calcGas_v();
      Gas_s1.setGasHeater(false);

      if (gas_val[0] > gas_MAX) {
        Gas_alarm = true;
      }
    }

    gCount++;

  }

  comms.RF_mode(rfWake);

  if (IR_alarm) {
    irmap = IR_s.IR_mapS(IR_flag);
    comms.sendTX(NODE_ID[0], irF, irmap, IR_vals);
  }
  else if (Gas_alarm) {
    comms.sendTX(NODE_ID[0], gasF, GasSensor, gas_val);
  }

  cleanUp();

}


/**
 * Setup Functions for:
 * a. IR Sensor
 * b. Dust Sensor
 * c. CO2 Sensor
 * d. Communication Module
 * e. Power Module
 */
void IR_setup(const byte clk[], const byte data[], const byte acq[]) {

  for (int i = 1; i < 5; i++) {

    IR_s.setIR(acq[i - 1], clk[i - 1], data[i - 1], i);

  }
}

void Dust_setup(const byte dust[], const long dust_th[]) {

  DST_s1.setDust(dust[0], dust[1]);
  DST_s1.setDustTh(dust_th[0], dust_th[1], dust_th[2]);

}

void Gas_setup(const byte gas[], const long th[]) {
  Gas_s1.setGas(gas[0], gas[1], th[0], th[1]);

}

void CO2_setup(const byte initCO2[], const float calib[], const long th[]) {

  CO2_s1.setCO2(initCO2[0], initCO2[1], calib[3]);
  CO2_s1.setCO2_th(th[0], th[1]);
  CO2_s1.setCO2_Calib(calib[0], calib[1], calib[2]);

}

int Batt_setup(const byte batt[]) {
  return 1;
}

void Comms_setup(const byte _comms[]) {

  comms.setComms(_comms[0], _comms[1]);

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

