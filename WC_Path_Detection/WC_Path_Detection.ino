/*
 * 
 * Algorithm: 
 * 
 * 1. Local node (LN): On sensor trigger:
 * 
 *  1.a. Wake up radio from sleep.
 *  1.b. Broadcast a wake-up beacon to Connected Stream.
 *  1.c. Wait for network association.  
 *  
 * 2. Remote node (RN): On receiving radio wake beacon: 
 *    
 *  2.a. If sensors triggered:
 *    2.a.1. Transmit path_detect_init frame to LN.
 *    2.a.2. Current RN becomes LN and hence perform tasks in 1.   
 *    2.a.3. 
 *    
 *  
 * Outgoing frames consists of path resolution + node details. 
 */


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
