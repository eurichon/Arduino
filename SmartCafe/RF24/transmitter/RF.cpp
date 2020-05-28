#include "RF.h"


Transceiver::Transceiver(int _ce, int _csn, RFType type){
  // rf default settings
  radio = new  RF24(_ce, _csn);
  radio->begin();
  radio->setPALevel(RF24_PA_MIN);
  radio->setAutoAck(false);
  this->type = type;
  
  radio->stopListening();
  
  state = RFState::idle;
  msg_id = 0;
  
  t = millis();
}



void Transceiver::update(){
  switch(state){
    case RFState::idle:
      memset(request, 0, 32 * sizeof(byte));
      retransmittions = 0;
      break;
    case RFState::toSend:
      //transmit for the first time
      answer_received = false;
      
      new_msg = false;
      
      radio->openWritingPipe(address[0]);
      radio->openReadingPipe(1,address[1]);
      
      radio->write(&request, sizeof(request));
      
      radio->openWritingPipe(address[1]);
      radio->openReadingPipe(1,address[0]);
      radio->startListening();
      
      state = RFState::toReceive;
      t = millis();
      break;
    case RFState::toReceive:
      if((millis() - t) < TIME_OUT){
        //for time TIME-OUT wait for an answer
        if(radio->available()){
          // to check answer
          radio->read(&anwser, sizeof(anwser));

          if(anwser[1] == MASTER_ADDRESS && anwser[2] == msg_id){
            answer_received = true;
            new_msg = true;
          }
        }
      }else{
        radio->stopListening();
        if(answer_received){
          // we got an answer - process succesfull
          state = RFState::idle;
        }else{
          if(retransmittions < RETRIES){
            // retry to send the message
            retransmittions++;
            state = RFState::toSend;
          }else{
            //exceed max re-transmittions - abort process
            state = RFState::idle;
          }
        }
      }
      break;
    default:
      break;
  }
}


bool Transceiver::isSuccesfull(){
  if(new_msg){
    new_msg = false;
    return true;
  }else{
    return false;
  }
}


bool Transceiver::sendMsg(byte *msg, uint8_t dest){
  if(state == RFState::idle){
    msg_id = (msg_id + 1) % 255;
    //if we are on idle mode send the msg
    request[0] = MASTER_ADDRESS;
    request[1] = dest;
    request[2] = msg_id;
    copy(&request[3], msg, 29);
    
    state = RFState::toSend;
    return true;
  }else{
    // else dont send the msg while you receiving
    return false;
  }
}


void Transceiver::getAnswer(byte *msg){
  copy(msg, anwser, 32);
}


void Transceiver::copy(byte *dest, byte *src, int size){
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
