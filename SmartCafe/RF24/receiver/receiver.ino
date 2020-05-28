/*
  Arduino Wireless Communication Tutorial
      Example 1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include "Receiver.h"

Receiver *rf;



void setup() {
  Serial.begin(9600);
  rf = new Receiver(CE, CSN, 0b00000001);
  
}

void loop() {
  rf->update();
  if(rf->msgReceived()){
    char msg[32]; 
    rf->getMesg(msg);
    
    char ack[29];
    
    switch(msg[0]){
      case 7:
        ack[0] = 255;
        break;
      default:
        break;
    }


    
    
    rf->sendAck(ack); 
    
    
  }
 
}
