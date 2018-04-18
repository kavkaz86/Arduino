/*
* Copyright (C) Libelium Comunicaciones Distribuidas S.L.
 * http://www.libelium.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Version: 0.1
 * Design: David Gascon
 * Implementation: Manuel Calvo, Octavio Benedi
 * Improved by: Joaquin Ruiz
 * gcc -o zigbeeStorer $(mysql_config --cflags) zigbeeStorer.c $(mysql_config --libs)
 */
#include "zigbeeStorer.h"
/*
* Main function:
 * - init structures and open serial port
 * - read frames from serial port
 * - parse each frame
 * - check places to store data & save data
 */
int main(int argc, char *argv[]) {
  //FILE *file;
  //char fileName[100];
  init(argc, argv);
  while (1)
  {
    getFrame();
    parseFrame(&strct_frame);
    /*
* This file is created by ManagerSystem when "save frames in a local file" is selected.
     * This file contains filename of the file to be written
     * THIS FILE IS USED LIKE A FLAG: if the file exits the function is executed
     */
    //file = fopen("/var/www/ManagerSystem/plugins/b_SensorData/b0_capturer/data/localFile","r");
    //if (file != NULL)
    //{
    //fgets(fileName,100,file);
    //fclose ( file );
    //storeInAFile(fileName, &strct_frame);
    //}
    /*
* This file is created by ManagerSystem while "show me now" is running to be written
     * THIS FILE IS USED LIKE A FLAG: if the file exits the function is executed
     */
    //file = fopen("/var/www/ManagerSystem/plugins/b_SensorData/b0_capturer/data/showMeNowCheck","r");
    //if (file != NULL)
    //{
    //fclose ( file );
    //showMeNowFile(&strct_frame);
    //}
    /*
* This file is created by ManagerSystem when "save frames in a local database" is selected.
     * If this (empty) file exists means that frames are being stored in a local database
     * THIS FILE IS USED LIKE A FLAG: if the file exits the function is executed
     */
    //FILE *fileDB;
    //fileDB = fopen("/var/www/ManagerSystem/plugins/b_SensorData/b0_capturer/data/localDB","r");
    //if (fileDB != NULL)
    //{
    //storeInADB(&strct_frame, LOCAL);
    //fclose ( fileDB );
    //}
    /*
* This file is created by ManagerSystem when "save frames in a external database" is selected.
     * If this (empty) file exists means that frames are being stored in a external database
     * THIS FILE IS USED LIKE A FLAG: if the file exits the function is executed
     */
    //fileDB = fopen("/var/www/ManagerSystem/plugins/b_SensorData/b0_capturer/data/extDB","r");
    //if (fileDB != NULL)
    //{
    //storeInADB(&strct_frame, EXTERNAL);
    //fclose ( fileDB );
    //}
  }
  return 0;
}
/*
* Init function:
 * - open serial port
 * - init structures
 */
int init(int argc, char *argv[]) {
  serialPort = serialPort0;
  /*
* Try to open serial port
   */
  if ((sd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY)) == -1)
  {
    fprintf(stderr, "Unable to open the serial port %s - \n", serialPort);
    exit(-1);
  }
  else
  {
    if (!sd)
    {
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
  return 0;
}
/*
* getFrame function:
 * - read bytes from serial port
 * - build a string from bytes until '\n'
 */
void getFrame() {
  char c;
  int j = 0;
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
      break;
    }
  }
}
/*
* parseFrame function:
 * - take the string that returns getFrame function
 * - separate each values from each frame into diferents variables
 * - example frame: [HEADER] -mac:0013a20040307f9c -x:27,y:23,z:1023 -temp:28 -bat:97%
 * THIS FUNTION MAY BE CHANGED BY THE USER
 */
void parseFrame(struct xbee_frame *p_frame) {
  char *_macStart;
  char *_xStart;
  char *_yStart;
  char *_zStart;
  char *_tempStart;
  char *_batStart;
  int index, indexEnd;
  char *aux2;
  if (((aux2 = strstr(aux, "-mac"))!= NULL)&&(strlen(aux)>50)){
    //Get mac value
    _macStart=strchr(aux2,':');
    index=_macStart-aux2;
    strncpy(p_frame->mac,aux2+index+1,16);
    p_frame->mac[16] = '\0';
    index = index+2+16+4;
    //Get acc.x value
    _xStart=strchr(aux2+index,':');
    indexEnd=_xStart-aux2;
    strncpy(p_frame->x,aux2+index-1, indexEnd-index-1);
    p_frame->x[indexEnd-index-1] = '\0';
    index = indexEnd+1;
    //Get acc.y value
    _yStart=strchr(aux2+index,':');
    indexEnd=_yStart-aux2;
    strncpy(p_frame->y,aux2+index, indexEnd-index-2);
    p_frame->y[indexEnd-index-2] = '\0';
    index = indexEnd+1;
    //Get acc.z value
    _zStart=strchr(aux2+index,':');
    indexEnd=_zStart-aux2;
    strncpy(p_frame->z,aux2+index, indexEnd-index-6);
    p_frame->z[indexEnd-index-6] = '\0';
    index = indexEnd+1;
    //Get temperature value
    _tempStart=strchr(aux2+index,':');
    indexEnd=_tempStart-aux2;
    strncpy(p_frame->temp,aux2+index, indexEnd-index-5);
    p_frame->temp[indexEnd-index-5] = '\0';
    index = indexEnd+1;
    //Get battery level
    _batStart=strchr(aux2+index,'\0');
    indexEnd=_batStart-aux2;
    strncpy(p_frame->bat,aux2+index, indexEnd-index);
    p_frame->bat[indexEnd-index] = '\0';
  }
  else{
    strcpy(p_frame->frame,aux);
  }
}
/*
* storeInAFile function:
 * - take the string that returns getFrame function
 * - separate each values from each frame into diferents variables
 * - store the values along with the time stamp in the file path located in "fileName"
 * THIS FUNTION MAY BE CHANGED BY THE USER
 */
int storeInAFile(char * fileName, struct xbee_frame *p_frame) {
  //Get date time
  time_t _time = time(0);
  struct tm *tlocal = localtime(&_time);
  char output[128];
  strftime(output,128,"%y-%m-%d %H:%M:%S",tlocal);
  //Make the full path to the file that will be written
  FILE *fp;
  char * _path = "/mnt/user/zigbee_data/";
  char path[200] = "/mnt/user/zigbee_data/";
  int lenPath = strlen(_path);
  int lenFile = strlen(fileName);
  int max = lenPath+lenFile-1;
  int i = lenPath;
  int cont = 0;
  for(i = lenPath; i < max;i++)
  {
    path[i] = fileName[cont];
    cont++;
  }
  //Write data into file
  fp = fopen ( path, "a" );
  fputs( "20", fp );
  fputs( output, fp );
  if (strlen(p_frame->mac)>1){
    fputs( " -mac:", fp );
    fputs( p_frame->mac, fp );
    fputs( " -x:", fp );
    fputs( p_frame->x, fp );
    fputs( " -y:", fp );
    fputs( p_frame->y, fp );
    fputs( " -z:", fp );
    fputs( p_frame->z, fp );
    fputs( " -temp:", fp );
    fputs( p_frame->temp, fp );
    fputs( " -bat:", fp );
    fputs( p_frame->bat, fp );
    // fputs( "\n", fp );
  }
  else{
    fputs( " -frame:", fp);
    fputs( p_frame->frame, fp);
    // fputs( "\n", fp );
  }
  fclose ( fp );
  return 0;
}
/*
* showMeNowFile function:
 * - write data into a temporal file
 * - this is used by the manager system in order to show in real time
 * THIS FUNTION MAY BE CHANGED BY THE USER
 */
int showMeNowFile(struct xbee_frame *p_frame) {
  FILE *fpp;
  fpp = fopen ( "/mnt/user/zigbee_data/.showMeNowFile", "w" );
  if (strlen(p_frame->mac)>1){
    fputs( "<b>MAC: </b>", fpp );
    fputs( p_frame->mac, fpp );
    fputs( "<br><b>Acc-X: </b>", fpp );
    fputs( p_frame->x, fpp );
    fputs( "<br><b>Acc-Y: </b>", fpp );
    fputs( p_frame->y, fpp );
    fputs( "<br><b>Acc-Z: </b>", fpp );
    fputs( p_frame->z, fpp );
    fputs( "<br><b>Temperature: </b>", fpp );
    fputs( p_frame->temp, fpp );
    fputs( "<br><b>Battery: </b>", fpp );
    fputs( p_frame->bat, fpp );
  }
  else{
    fputs( "<b>Frame: </b>", fpp );
    fputs( p_frame->frame, fpp );
  }
  fputs( "<br>", fpp );
  fclose ( fpp );
  return 0;
}
/*
* storeInADB function:
 * - used to write data into an local or external database
 * - get connection info (database, table, ip, user, ...) from a file
 * - insert data into selected database
 *
 * THIS FUNTION MAY BE CHANGED BY THE USER
 */
int storeInADB(struct xbee_frame *p_frame, int to) {
  char localDBname[100]="";
  char localDBtable[100]="";
  char localDBip[100]="";
  char localDBport[100]="";
  char localDBuser[140]="";
  char localDBpass[140]="";
  char local_DBname[100]="";
  char local_DBtable[100]="";
  char local_DBip[100]="";
  char local_DBport[100]="";
  char local_DBuser[140]="";
  char local_DBpass[140]="";
  char * YYY;
  //Init mysql database
  mysql_init(&mysql);
  mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "your_prog_name");
  /*Read that contains connection info ("to" variable represents LOCAL or EXTERNAL database)
   EXAMPLE OF CONNECTION INFO FILE
   MeshliumDB
   zigbeeData
   192.168.1.19
   3306
   root
   libelium2007
   */
  FILE *file;
  if (to == 0) {
    file = fopen("/mnt/lib/cfg/zigbeeDBSetup","r");
  }
  else {
    file = fopen("/var/www/ManagerSystem/plugins/b_SensorData/b0_capturer/data/ExtDataConnection","r");
  }
  //Get connection info
  if (file != NULL)
  {
    fgets(localDBname,100,file);
    fgets(localDBtable,100,file);
    fgets(localDBip,100,file);
    fgets(localDBport,100,file);
    fgets(localDBuser,100,file);
    fgets(localDBpass,100,file);
    fclose ( file );
  }
  //Clear data of connection of '\n' chars
  YYY=strchr(localDBname, '\n');
  int reend=YYY-localDBname;
  strncpy(local_DBname,localDBname,reend);
  YYY=strchr(localDBtable, '\n');
  reend=YYY-localDBtable;
  strncpy(local_DBtable,localDBtable,reend);
  YYY=strchr(localDBip, '\n');
  reend=YYY-localDBip;
  strncpy(local_DBip,localDBip,reend);
  YYY=strchr(localDBport, '\n');
  reend=YYY-localDBport;
  strncpy(local_DBport,localDBport,reend);
  YYY=strchr(localDBuser, '\n');
  reend=YYY-localDBuser;
  strncpy(local_DBuser,localDBuser,reend);
  YYY=strchr(localDBpass, '\n');
  reend=YYY-localDBpass;
  strncpy(local_DBpass,localDBpass,reend);
  //Insert data into selected database
  if (!mysql_real_connect(&mysql, local_DBip, local_DBuser, local_DBpass, local_DBname, atoi(local_DBport), NULL, 0))
  {
    fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
    exit(-1);
  }
  else
  {
    fprintf(stderr, "Connected to database: return: %s\n", mysql_error(&mysql));
  }
  sprintf(sql, "insert into %s values(NULL,now(),'%s','%s','%s','%s','%s','%s','%s');", local_DBtable, p_frame->mac, p_frame->x, p_frame->y, p_frame->z, p_frame->temp, p_frame->bat, p_frame->frame);
  if (mysql_query(&mysql, sql) != 0) {
    fprintf(stderr, "BBDDFailed:%s\n", sql);
  } 
  else {
    fprintf(stdout, "BBDDSucceeded:%s\n", sql);
  }
  mysql_close(&mysql);
  return 0;
}

