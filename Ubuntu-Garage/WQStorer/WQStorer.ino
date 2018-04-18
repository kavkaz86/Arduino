#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <termios.h> /* Terminal control library (POSIX) */
#include <ctype.h>
#include <mysql.h>

#define MAX 500
#define MAX_SQL 700

int main(int argc, char *argv[]) {
  int sd = 3;
  char *serialPort = "";
  char *serialPort0 = "/dev/ttyS0";
  char *serialPort1 = "/dev/ttyS1";
  char *USBserialPort0 = "/dev/ttyUSB0";
  char *USBserialPort1 = "/dev/ttyUSB1";
  char valor[MAX] = "";
  char aux[MAX] = "";
  char c;
  struct termios opciones;
  char *s0 = "S0";
  char *s1 = "S1";
  char *u0 = "USB0";
  char *u1 = "USB1";

  /**************************************
   * Defines para crear la insercion SQL
   *************************************/
  char sql[MAX_SQL] = "";
  MYSQL mysql;
  /* STARTING MAIN */
  if (argc != 3) {
    fprintf(stderr, "Usage: %s port bbdd_ip\n", argv[0]);
    exit(0);
  }


  if (!strcmp(argv[1], s0)) {
    serialPort = serialPort0;
  }
  if (!strcmp(argv[1], s1)) {
    serialPort = serialPort1;
  }
  if (!strcmp(argv[1], u0)) {
    serialPort = USBserialPort0;
  }
  if (!strcmp(argv[1], u1)) {
    serialPort = USBserialPort1;
  }
  if (!strcmp(serialPort, "")) {
    fprintf(stderr, "Choose a valid port (S0, S1, USB0, USB1)\n");
    exit(0);
  }

  if ((sd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY)) == -1) {
    fprintf(stderr, "Unable to open the serial port %s - \n", serialPort);
    exit(-1);
  } 
  else {
    if (!sd) {
      sd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);
    }
    fcntl(sd, F_SETFL, 0);
  }

  tcgetattr(sd, &opciones);
  cfsetispeed(&opciones, B38400);
  cfsetospeed(&opciones, B38400);
  opciones.c_cflag |= (CLOCAL | CREAD);
  /*
* No parity
   */
  opciones.c_cflag &= ~PARENB;
  opciones.c_cflag &= ~CSTOPB;
  opciones.c_cflag &= ~CSIZE;
  opciones.c_cflag |= CS8;
  /*
* raw input:
   * making the applycation ready to receive
   */
  opciones.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  /*
* Ignore parity errors
   */
  opciones.c_iflag |= ~(INPCK | ISTRIP | PARMRK);
  opciones.c_iflag |= IGNPAR;
  opciones.c_iflag &= ~(IXON | IXOFF | IXANY | IGNCR | IGNBRK);
  opciones.c_iflag |= BRKINT;
  /*
* raw output
   * making the applycation ready to transmit
   */
  opciones.c_oflag &= ~OPOST;
  /*
* apply
   */
  tcsetattr(sd, TCSANOW, &opciones);
  mysql_init(&mysql);
  mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "your_prog_name");
  if (!mysql_real_connect(&mysql, argv[2], "root", "quandour1", "WQ", 0, NULL, 0)) {
    fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
    exit(-1);
  } 
  else {
    fprintf(stderr, "Connected to database: return: %s\n", mysql_error(&mysql));
  }


  int j = 0;
  int ret;
  int ret2;
  char sn1[50];
  char sn2[50];
  char sn3[50];
  char sn4[50];
  char sn5[50];
  char sn6[50];
  char sn7[50];
  char sn8[50];
  char na[10];
  char rssi[10];
  char paqID[10];
  char* emsg = "none";
  char h1[100]; 
  char h2[100];
  //Get frame char by char untill '\n'
  while (1)
  {
    read(sd,&c,1);
    if((c=='\0') || (c=='\r'))
    {
      //Strange chars will be modified
      aux[j]='_';
    }
    else
    {
      aux[j]=c;
      j++;
    }
    if((c=='\n') || (j==(MAX-1)))
    {
      aux[j]='\0';
      ret = sscanf(aux,"%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#", &h1[0], &sn1[0], &sn2[0], &sn3[0], &sn4[0], &sn5[0], &sn6[0]); 
      if (ret == 7){
        fprintf(stderr, "EC: %s\n TDS: %s\n SAL: %s\n OP: %s\n mgL: %s\n ph: %s\n \n", sn1, sn2, sn3, sn4, sn5, sn6);
        sprintf(sql, "insert into data values(NULL,now(),'%s','%s','%s','%s','%s','%s');", sn1, sn2, sn3, sn4, sn5, sn6);
        if (mysql_query(&mysql, sql) != 0) {
          fprintf(stderr, "BBDDFailed:%s\n", sql);
        } 
        else {
          fprintf(stdout, "BBDDSucceeded:%s\n", sql);
        }
      }
      else {
        fprintf(stderr, "\nFrame: %s\n", valor);
        fprintf(stderr, "\nret:%d\n %s %s %s %s %s %s %s %s %s %s", ret, na, paqID, rssi, sn1, sn2, sn3, sn4, sn5, sn6, sn7);
        fprintf(stderr, "\nBad frame received\n");
      }
      j = 0;
      aux[j] = '\0';
      
    }
  }
  mysql_close(&mysql);
  close(sd);
  exit(0);
}



