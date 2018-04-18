 #include "Streaming.h"
 #include "SoftwareSerial.h"
 
 int ledPin = 13;                  // LED test pin
 int rxPin = 0;                    // RX PIN 
 int txPin = 1;                    // TX TX
 int byteGPS=-1;
 char linea[300] = "";
 char comandoGPR[7] = "$GPGGA";
 int cont=0;
 int bien=0;
 int conta=0;
 int indices[13];
 SoftwareSerial xbee(2,3); 
 
 void setup() {
   pinMode(ledPin, OUTPUT);       // Initialize LED pin
   pinMode(rxPin, INPUT);
   pinMode(txPin, OUTPUT);
   Serial.begin(4800);
   xbee.begin(9600); 
   
   for (int i=0;i<300;i++){       // Initialize a buffer for received data
     linea[i]=' ';
   }   
 }

 void loop() {
   digitalWrite(ledPin, HIGH);
   byteGPS=Serial.read();         // Read a byte of the serial port
   if (byteGPS == -1) {           // See if the port is empty yet
     delay(100); 
   } else {
     linea[conta]=byteGPS;        // If there is serial port data, it is put in the buffer
     conta++;                      
     Serial << _BYTE(byteGPS); 
     if (byteGPS==13){            // If the received byte is = to 13, end of transmission
       digitalWrite(ledPin, LOW); 
       cont=0;
       bien=0;
       for (int i=1;i<7;i++){     // Verifies if the received command starts with $GPR
         if (linea[i]==comandoGPR[i-1]){
           bien++;
         }
       }
       if(bien==6){               // If yes, continue and process the data
         for (int i=0;i<300;i++){
           if (linea[i]==','){    // check for the position of the  "," separator
             indices[cont]=i;
             cont++;
           }
           if (linea[i]=='*'){    // ... and the "*"
             indices[12]=i;
             cont++;
           }
         }
        
         for (int i=0;i<12;i++){
           switch(i){
             case 0 :xbee.println("Time in UTC (HhMmSs): ");break;
             case 1 :xbee.println("Status (A=OK,V=KO): ");break;
             case 2 :xbee.println("Latitude: ");break;
             case 3 :xbee.println("Direction (N/S): ");break;
             case 4 :xbee.println("Longitude: ");break;
             case 5 :xbee.println("Direction (E/W): ");break;
             case 6 :xbee.println("Velocity in knots: ");break;
             case 7 :xbee.println("Heading in degrees: ");break;
             case 8 :xbee.println("Date UTC (DdMmAa): ");break;
             case 9 :xbee.println("Magnetic degrees: ");break;
             case 10 :xbee.println("(E/W): ");break;
             case 11 :xbee.println("Mode: ");break;
             case 12 :xbee.println("Checksum: ");break;
           }
           for (int j=indices[i];j<(indices[i+1]-1);j++){
             xbee.print(linea[j+1]); 
           }
           xbee.println("");
         }
         xbee.println("---------------");
         delay(2000);
       }
       conta=0;                    // Reset the buffer
       for (int i=0;i<300;i++){    //  
         linea[i]=' ';             
       }                 
     }
   }
   
   
   
 }

