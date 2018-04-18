/*/
 * Bushfire Monitoring System S/W for Remote Sensing Nodes
 * Author: Amro Qandour
 * S/W rev. : 1.0
 * Date: 07/07/15
 */

#include  "Streaming.h"
#include "FireHound.h"

/*/
 * Tx-Frame Types:
 * 1: Reporting frame
 * 2: Primary sensor detection frame
 * 3: Secondary sensor detection frame
 * 4: Diagnostics frame
 */
#define rep_frame 1
#define ir_frame  2
#define gas_frame 3
#define diag_frame  4

/*/
 * IR Sensor Mapping:
 * 0:Inactive - 1:Active
 * IR[IR_1, IR_2, IR_3, IR_4]
 * 
 *        IR_1  IR_2  IR_3  IR_4  : IR_MAP  
           0    0     0     0       IR_0
           0    0     0     1       IR_4
           0    0     1     0       IR_3
           0    0     1     1       IR_34
           0    1     0     0       IR_2
           0    1     0     1       IR_24
           0    1     1     0       IR_23
           0    1     1     1       IR_234
[IR_MAP] = 1    0     0     0       IR_1
           1    0     0     1       IR_14
           1    0     1     0       IR_13
           1    0     1     1       IR_134
           1    1     0     0       IR_12
           1    1     0     1       IR_124
           1    1     1     0       IR_123
           1    1     1     1       IR_1234
 */
#define IR_0  0
#define IR_1  1 
#define IR_2  2
#define IR_3  3
#define IR_4  4
#define IR_1234 5 
#define IR_123  6
#define IR_124  7
#define IR_12 8
#define IR_134  9 
#define IR_13 10
#define IR_14 11
#define IR_234  12
#define IR_23 13  
#define IR_24 14
#define IR_34 15
#define IR_MAX  125

/*/
 * IR Clock Pin Assignment:
 * IR_clk_pins[Clk_1,Clk_2,Clk_3,Clk_4]
 */
static const byte IR_clk[4] = {2,3,4,5};

/*/
 * IR Data Pin Assignment:
 * IR_data[data_1,data_2,data_3,data_4]
 */
static const byte IR_data[4] = {6,7,8,9};

/*/
 * IR Active Pin Assignment:
 * IR_act[act_1,act_2,act_3,act_4]
 */
static const byte IR_acq[4] = {10,11,12,13};

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
 * Gas Sensor Pin Assignment:
 * Gas_sensor[data,EN]
 */
static const byte Gas_sensor[2] = {A2,17};

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
 * Others:
 */


static const byte NODE_ID[1] = {'1'};
static const TempUnit SCALE=CELSIUS;
bool IR_flag[5] = {1, 0, 0, 0, 0};
bool c[4] = {0, 0, 0, 0};
float IR_vals[4] = {0};

FHound IR_s1;
FHound IR_s2;
FHound IR_s3;
FHound IR_s4;
FHound DST_s1;
FHound CO2_s1;
FHound Gas_s1; 

void setup() {
  
  //int isIR = IR_setup(IR_clk, IR_data, IR_acq);
  IR_setup(IR_clk, IR_data, IR_acq);
  //int isDust = Dust_setup(Dust_sensor);
  Dust_setup(Dust_sensor, Dust_TH);
  int isGas = Gas_setup(Gas_sensor);
  int isBatt = Batt_setup(low_batt_ind);
  int isComms = Comms_setup(Comms_xbee);

  //Debug:
  //Serial << isIR << isDust << isGas << isBatt << isComms << endl;
  
}

void loop() {
  

}

void IR_setup(const byte clk[], const byte data[], const byte acq[]){
  
  IR_s1.setIR(acq[0], clk[0], data[0],1);
  IR_s2.setIR(acq[1], clk[1], data[1],2);
  IR_s3.setIR(acq[2], clk[2], data[2],3);
  IR_s4.setIR(acq[3], clk[3], data[3],4);
}

void Dust_setup(const byte dust[], const long dust_th[]){

  DST_s1.setDust(dust[0], dust[1]); 
  DST_s1.setDustTh(dust_th[0], dust_th[1], dust_th[2]);
  
}

int Gas_setup(const byte gas[]){
  return 1;
}

int Batt_setup(const byte batt[]){
  return 1; 
}

int Comms_setup(const byte comms[]){
  return 1;
}

int check_IR(const float data[]) {

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
  return -1;
}

int IR_mapS(const bool array[]) {

  const bool a0 = array[0];

  if (array[1] != a0 && array[2] != a0 && array[3] != a0 && array[4] != a0) {
    return IR_0;
  }

  if (array[1] != a0 && array[2] != a0 && array[3] != a0 && array[4] == a0) {
    return IR_4;
  }

  if (array[1] != a0 && array[2] != a0 && array[3] == a0 && array[4] != a0) {
    return IR_3;
  }

  if (array[1] != a0 && array[2] != a0 && array[3] == a0 && array[4] == a0) {
    return IR_34;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] != a0 && array[4] != a0) {
    return IR_2;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] != a0 && array[4] == a0) {
    return IR_24;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] == a0 && array[4] != a0) {
    return IR_23;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] == a0 && array[4] == a0) {
    return IR_234;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] != a0 && array[4] != a0) {
    return IR_1;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] != a0 && array[4] == a0) {
    return IR_14;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] == a0 && array[4] != a0) {
    return IR_13;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] == a0 && array[4] == a0) {
    return IR_134;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] != a0 && array[4] != a0) {
    return IR_12;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] != a0 && array[4] == a0) {
    return IR_124;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] == a0 && array[4] != a0) {
    return IR_123;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] == a0 && array[4] == a0) {
    return IR_1234;
  }

}

int tx_frame(char n_id, const int f_id, const int ir_map, const float data[]) {
  char  sens_IR_1[10];
  String  data_out=""; 
  for(int i=0; i<4; i++){
    if(i==0){
      data_out="[";
    }
    data_out+=data[i];
    if(i<3){
      data_out+=",";
    }
    if(i==3){
      data_out+="]";
    }
  }
  
  
  
  switch (f_id) {

    case  rep_frame:
      Serial << "Reporting Frame:\t" << f_id << "," << n_id << "," <<  data_out <<  endl;  
      break;

    case  ir_frame:
      Serial << "IR Frame:\t\t" << f_id << "," << n_id << "," << ir_map << "," <<  data_out << endl;  
      break;

    case  gas_frame:
      Serial << "Gas Frame:\t\t" << f_id << "," << n_id << "," << data_out << endl;  
      break;

    case  diag_frame:
      Serial << "Diagnostic Frame:\t" << f_id << "," << n_id << "," << SENS_REQ << endl;  
      break;

    default:
      return -1;  
  }
  return 1;
}

