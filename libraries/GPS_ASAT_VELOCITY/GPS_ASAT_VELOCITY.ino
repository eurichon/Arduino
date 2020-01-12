#include <SoftwareSerial.h>
#include "GPS_library.h"



// setup gps serial
int gpsTxPin = 8; //prasino
int gpsRxPin = 9; //kitrino
SoftwareSerial gpsSerial(gpsTxPin, gpsRxPin);
GPS gps;

void setup()
{
  Serial.begin(9600);  //set monitor to 9600
 
  gpsSerial.begin(9600); //adjust for GPS unit

  //gps.print_back();
  gps.print_back();


  Serial.println("Ready!");
}

void loop()
{
  while(gpsSerial.available()){
    char c = gpsSerial.read();
    gps.GPS_update(c);
  }
  
//  if(gps.is_ended){
//    Serial.println("**********************************************************");
//    Serial.println(gps.getSpeed());
//    Serial.println(gps.getTime());
//    Serial.println(gps.getAltitude());    
//    gps.is_ended = false;
//  }
}


