#include  "Streaming.h"
#define  rep_frame 1
#define ir_frame  2
#define gas_frame 3
#define diag_frame  4
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
#define NODE_ID '1'
static const float IR_MAX = 125.0;
//char NODE_ID[1] = '1';
bool IR_flag[5] = {1, 0, 0, 0, 0};
bool c[4] = {0, 0, 0, 0};
float IR_vals[4] = {0};

void setup() {
  Serial.begin(9600);

}

void loop() {

  /*/ --------------------------------------------IR FLAG ROUTINE TEST------------------------------------------------ /*/
  Serial.println("----IR FLAG TEST----");
  Serial.println();
  Serial << "IR_FLAG" << '\t' << "IR_MAP" << endl;
  setflag(0, 0, 0, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(0, 0, 0, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(0, 0, 1, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(0, 0, 1, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(0, 1, 0, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(0, 1, 0, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(0, 1, 1, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(0, 1, 1, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 0, 0, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 0, 0, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 0, 1, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 0, 1, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 1, 0, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 1, 0, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 1, 1, 0);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(100);

  setflag(1, 1, 1, 1);
  Serial << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t' << check1(IR_flag) << endl;
  delay(5000);
  Serial.println();
  /*/ ------------------------------------------------------------------------------------------------------------ /*/

  /*/ --------------------------------------------IR FLAG ROUTINE TEST------------------------------------------------ /*/
  Serial.println("----IR_VALS + FLAG CHECK ROUTINE TEST----");
  Serial.println();
  Serial << "IR_VALS" << '\t' << '\t' << '\t' << '\t' << "IR_FLAG" << '\t' << '\t' << "IR_MAP" << endl;
  setflag(0, 0, 0, 0);
  setIR_vals(0.0, 0.0, 0.0, 0.0);
  //Serial << "IR_VALS= " << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << endl;
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(0.0, 0.0, 0.0, 150.0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(0.0, 0.0, 150.0, 0.0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(0, 0, 150, 150);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(0, 150, 0, 0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(0, 150, 0, 150);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(0, 150, 150, 0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(0, 150, 150, 150);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 0, 0, 0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 0, 0, 150);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 0, 150, 0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 0, 150, 150);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] <<  '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 150, 0, 0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 150, 0, 150);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] << '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 150, 150, 0);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] <<  '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  delay(100);

  setIR_vals(150, 150, 150, 150);
  if (check_IR(IR_vals)) {
    Serial << IR_vals[0] << "," << IR_vals[1] << "," << IR_vals[2] << "," << IR_vals[3] <<  '\t' << IR_flag[1] << IR_flag[2] << IR_flag[3] << IR_flag[4] << '\t'  << '\t' << check1(IR_flag) << endl;
  }
  setflag(0, 0, 0, 0);
  //delay(100);

  delay(5000);
  Serial.println();
  /*/ ------------------------------------------------------------------------------------------------------------ /*/
  Serial.println("----Tx Frame TEST----");
  Serial.println();
  setIR_vals(150, 150, 150, 150);
  check_IR(IR_vals);
  int irmap = check1(IR_flag);
  tx_frame(NODE_ID, ir_frame, irmap, IR_vals);
  delay(5000);
  Serial.println();
  
}


void setflag(const bool a, const bool b, const bool c, const bool d ) {

  IR_flag[1] = a;
  IR_flag[2] = b;
  IR_flag[3] = c;
  IR_flag[4] = d;
}

void setIR_vals(const float a, const float b, const float c, const float d) {

  IR_vals[0] = a;
  IR_vals[1] = b;
  IR_vals[2] = c;
  IR_vals[3] = d;

}

void clearIR_vals() {

  for (int i = 0 ; i++; i < 4) {

    IR_vals[i] = '\0';

  }
}

bool check(const bool array[], int n)
{
  const bool a0 = array[0];
  for (int i = 1; i < n; i++)
  {
    if (array[i] != a0)
      return true;
  }
  return false;
}

int check1(const bool array[]) {

  const bool a0 = array[0];

  if (array[1] != a0 && array[2] != a0 && array[3] != a0 && array[4] != a0) {
    return 0;
  }

  if (array[1] != a0 && array[2] != a0 && array[3] != a0 && array[4] == a0) {
    return 1;
  }

  if (array[1] != a0 && array[2] != a0 && array[3] == a0 && array[4] != a0) {
    return 2;
  }

  if (array[1] != a0 && array[2] != a0 && array[3] == a0 && array[4] == a0) {
    return 3;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] != a0 && array[4] != a0) {
    return 4;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] != a0 && array[4] == a0) {
    return 5;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] == a0 && array[4] != a0) {
    return 6;
  }

  if (array[1] != a0 && array[2] == a0 && array[3] == a0 && array[4] == a0) {
    return 7;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] != a0 && array[4] != a0) {
    return 8;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] != a0 && array[4] == a0) {
    return 9;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] == a0 && array[4] != a0) {
    return 10;
  }

  if (array[1] == a0 && array[2] != a0 && array[3] == a0 && array[4] == a0) {
    return 11;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] != a0 && array[4] != a0) {
    return 12;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] != a0 && array[4] == a0) {
    return 13;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] == a0 && array[4] != a0) {
    return 14;
  }

  if (array[1] == a0 && array[2] == a0 && array[3] == a0 && array[4] == a0) {
    return 15;
  }

}

int check_IR(const float data[]) {

  const float a = data[0];
  const float b = data[1];
  const float c = data[2];
  const float d = data[3];
  //bool a = false;
  /*
  if(IR_1 > IR_MAX){
    a = true;
  }
  if(IR_2 > IR_MAX){
    a = true;
  }
  if(IR_3 > IR_MAX){
    a = true;
  }
  if(IR_4 > IR_MAX){
    a = true;
  }*/
  //if(a){
  if ((a > IR_MAX) || (a > IR_MAX) || (c > IR_MAX) || (d > IR_MAX) ) {
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
/*/
 * 
 * #define IR_0  0
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

dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
 */
int tx_frame(char n_id, const int f_id, const int ir_map, const float data[]) {
  
   
  switch(ir_map){
    case  IR_0:
      break;

    case  IR_1:
      
      break;

    case  IR_2:
      break;

    case  IR_3:
      break;

    case  IR_4:
      break;

    case  IR_1234:
      break;

    case  IR_123:
      break;

    case  IR_124:
      break;

    case  IR_134:
      break;

    case  IR_13:
      break;

     case  IR_14:
      break;

    case  IR_234:
      break;

    case  IR_23:
      break;

    case  IR_24:
      break;

    case  IR_34:
      break;    
            
  }
  switch (f_id) {

    case  rep_frame:
      Serial << f_id << "," << n_id << "," <<  data[0] <<  endl;  
      break;

    case  ir_frame:
      Serial << f_id << "," << n_id << "," << "[" << data[0] << "," << data[1] << "," << data[2] << "," << data[3] << "]" << endl;  
      break;

    case  gas_frame:
      Serial << f_id << "," << n_id << "," << data[0] << endl;  
      break;

    case  diag_frame:
      Serial << f_id << "," << n_id << "," << data[0] << endl;  
      break;

    default:
      return -1;  
  }
  return 1;
}

void format_data(



