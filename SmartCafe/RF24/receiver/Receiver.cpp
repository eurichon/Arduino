#include "Receiver.h"


Receiver::Receiver(int _ce, int _csn, uint8_t id){
  // rf default settings
  radio = new  RF24(_ce, _csn);
  radio->begin();
  radio->setPALevel(RF24_PA_MIN);
  radio->setAutoAck(false);
  
  radio->startListening();
  radio->openWritingPipe(address[1]);
  radio->openReadingPipe(1,address[0]);
  
  state = RFState::toReceive;
  receiver_id = id;
  randomSeed(analogRead(A0));
  pre_msg_id = 0;
  
  t = millis();
}



void Receiver::update(){
  switch(state){
    case RFState::toReceive:
      //transmit for the first time
      if(radio->available()){
        memset(command_msg, 0, 32 * sizeof(byte));
        radio->read(&command_msg, sizeof(command_msg));

        //check if the message is valid
        if(command_msg[0] == MASTER_ADDRESS){
          int is_correct = command_msg[1] & receiver_id;
          // if the message targets this device and there is an new update msg id
          if(is_correct && pre_msg_id != command_msg[2]){
            pre_msg_id = command_msg[2];
            is_valid = true;
            state = RFState::toSend;

            radio->openWritingPipe(address[0]);
            radio->openReadingPipe(1,address[1]);
            radio->stopListening();
            time_offset = random(50, 160);
            t = millis();
          }
        }
      }
    
      break;
    case RFState::toSend:
      if(millis()-t > time_offset){
        radio->write(&ack_msg, sizeof(ack_msg));
        //printMsg(ack_msg);
        radio->startListening();
        radio->openWritingPipe(address[1]);
        radio->openReadingPipe(1,address[0]);
        state = RFState::toReceive;
      }
    
      
      break;
    default:
      break;
  }
}


void Receiver::getMesg(byte *msg){
  copy(msg, &command_msg[3], 29);
}


void Receiver::sendAck(byte *msg){
  ack_msg[0] = receiver_id;
  ack_msg[1] = MASTER_ADDRESS;
  ack_msg[2] = pre_msg_id;
  copy(&ack_msg[3], msg, 29);
}


bool Receiver::msgReceived(){
  if(is_valid){
    is_valid = false;
    return true;
  }else{
    return false;
  }
}


void Receiver::copy(byte *dest, byte *src, int size){
  for(int i = 0; i < size; i++){
    dest[i] = src[i];
  }
}


void printMsg(char *arr){
  for(int i = 0;i < 32;i++){
    Serial.print(arr[i],DEC);
    Serial.print(".");
  }
  Serial.println(" ");

  for(int i = 0;i < 32;i++){
    Serial.print(arr[i]);
    Serial.print(".");
  }
  Serial.println(" ");
}
