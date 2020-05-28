#ifndef RF_H
#define RF_H


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MASTER_ADDRESS  0x00
#define RETRIES         5
#define TIME_OUT        200
#define CE              7
#define CSN             8



typedef enum{
  Receiver,
  Transmitter
}RFType;

typedef enum{
  idle,
  toSend,
  toReceive,
}RFState;


void printMsg(char *arr);

class Transceiver{
  private:
    const uint64_t address[2] = { 0xABCDABCD71LL, 0x544d52687CLL };

    int retransmittions;
    bool answer_received, new_msg;
    byte request[32], anwser[32];
    RFState state;
    RFType type;
    RF24 *radio;
    unsigned long period, t;

    void copy(byte *dest, byte *src, int size);
    uint8_t msg_id;    
  public:
    Transceiver(int _ce, int _csn, RFType type);
    bool sendMsg(byte *msg, uint8_t dest);
    bool isSuccesfull();
    void getAnswer(byte *msg);
    void update();
};



#endif
