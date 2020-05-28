#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MSG_SIZE 20
#define TIME_OUT 200

RF24 radio(7, 8);

bool newData;
bool has_answered;
char receivedChar;
char receivedChars[MSG_SIZE];

const uint64_t address[2] = { 0xABCDABCD71LL, 0x544d52687CLL };
const char command_names[7][15] = {"Add", "Remove", "Update", "Set Light", "Set Temp", "Set Tent", "Give Data"};
const char type_names[4][15] = {"Coffee" , "Juice", "Beverages", "Snacks"};

uint8_t msg_id;
uint8_t device_id = 0x00;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setAutoAck(false);
  radio.stopListening();
  msg_id = 0;
  has_answered = false;
}

void loop() {
  // read serial input
  newData = false;
  recvWithEndMarker();
  if (newData) {
    // send command over rf24
    Serial.print("Input is: ");
    Serial.println(receivedChars);

    msg_id = (msg_id + 1) % 255;
    byte msg[32];
    msg[0] = msg_id;
    msg[1] = device_id;
    
    
    while(!has_answered){
      Serial.println("Retrasmitting");
      radio.openWritingPipe(address[0]);
      radio.openReadingPipe(1,address[1]);
  
      radio.write(&receivedChars, sizeof(receivedChars));
  
      radio.openWritingPipe(address[1]);
      radio.openReadingPipe(1, address[0]);
      radio.startListening();
  
      unsigned long t = millis();
      while ((millis() - t) < TIME_OUT) {
        if (radio.available()) {
          has_answered = true;
          char text[32] = "";
          radio.read(&text, sizeof(text));
          Serial.print("Receive answer: ");
          Serial.println(text);
        }
      }
      radio.stopListening();
    }
    
    newData = false;
  }
  has_answered = false;
}







void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= MSG_SIZE) {
        ndx = MSG_SIZE - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}


void readInput() {

}
