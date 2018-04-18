/*************PACKET SIZE*************/ 

#define  RF_psize_1  25

#define  RF_psize_2  50

#define  RF_psize_3  75

#define  RF_psize_4  100


/*************TX time delay*************/ 

#define  RF_tdelay_1  5

#define  RF_tdelay_2  10

#define  RF_tdelay_3  20

#define  RF_tdelay_4  30

#define  RF_tdelay_5  40

#define  RF_tdelay_6  50

#define  RF_tdelay_7  60

#define  RF_tdelay_8  70

#define  RF_tdelay_9  80

#define  RF_tdelay_10  90

#define  RF_tdelay_11  100

#define  RF_tdelay_12  150

#define  RF_tdelay_13  200

#define  RF_tdelay_14  300

#define  RF_tdelay_15  400

#define  RF_tdelay_16  500

#define  RF_tdelay_17  600

#define  RF_tdelay_18  700

#define  RF_tdelay_19  800

#define  RF_tdelay_20  900

#define  RF_tdelay_21  1000


/*************BAUD_RATE*************/ 

#define  RF_brate_1  9600  //default.

#define  RF_brate_2  38400

#define  RF_brate_3  115200


/*************PWR_AT_ADJUSTMENT*************/ 

#define  RF_pwr_1  ""  //LOWEST

#define  RF_pwr_2  ""

#define  RF_pwr_3  ""

#define  RF_pwr_4  ""

#define  RF_pwr_5  ""  //HIGHEST


void setup(void){
  
  Serial.begin(RF_brate_1);
  
}



