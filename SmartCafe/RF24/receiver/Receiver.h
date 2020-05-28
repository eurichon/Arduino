#ifndef RF_H
#define RF_H


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Arduino.h"

#define MASTER_ADDRESS  0x00
#define RETRIES         5
#define TIME_OUT        200
#define CE              7
#define CSN             8


void printMsg(char *arr);

typedef enum{
  toSend,
  toReceive,
}RFState;


class Receiver{
  private:
    const uint64_t address[2] = { 0xABCDABCD71LL, 0x544d52687CLL };
    
    bool is_valid;;
    byte command_msg[32], ack_msg[32];
    RFState state;
    RF24 *radio;
    unsigned long period, t;
    uint8_t receiver_id;
    uint8_t pre_msg_id;    
    unsigned long time_offset;

    void copy(byte *dest, byte *src, int size);
  public:
    Receiver(int _ce, int _csn, uint8_t id);
    bool msgReceived();
    void getMesg(byte *msg);
    void sendAck(byte *msg);
    void update();
};



#endif
