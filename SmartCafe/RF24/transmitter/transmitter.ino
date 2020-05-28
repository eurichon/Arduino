/*
  Arduino Wireless Communication Tutorial
      Example 1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include "RF.h"

//RF24 radio(PB0, PA4); // CE, CSN on Blue Pill
#define SCAN          7

Transceiver *rf;
unsigned long t;
bool newData;
int numChars = 29;
char receivedChars[29];

void setup() {
  Serial.begin(19200);
  while (!Serial);


  rf = new Transceiver(CE, CSN, RFType::Transmitter);
  newData = false;
  t = millis();
}

void loop() {
  recvWithEndMarker();

  if (newData) {
    rf->sendMsg(receivedChars, 0b11111111);
    newData = false;
  }






  rf->update();

  if (rf->isSuccesfull()) {
    char txt[32];
    rf->getAnswer(txt);
    Serial.write(200);
    for(int i = 0;i < 31;i++){
      if(txt[i] == 255){
        break;
      }else{
        Serial.write(txt[i]);
      }
    }
    

  }
}



void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = 255;
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
