#include <XBee.h>

/*
This example is for Series 2 XBee
 Receives a ZB RX packet and sets a PWM value based on packet data.
 Error led is flashed if an unexpected packet is received
 */

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

void setup() {
  // start serial
  Serial.begin(115200);
  xbee.begin(Serial);

}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE ) {
                xbee.getResponse().getRx16Response(rx16);
        }
        else if(xbee.getResponse().getApiId() == RX_64_RESPONSE ){
          xbee.getResponse().getRx64Response(rx64);
        }
        for (int i = 0; i < rx.getDataLength(); i++) { 
         Serial.print((char)rx.getData(i)); 
       } 
  }
}

