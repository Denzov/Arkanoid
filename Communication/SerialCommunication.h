#ifndef _COM_PORT_COMMUNICATION_H_
#define _COM_PORT_COMMUNICATION_H_

#define DEBUG

#include <iostream>
#include <string>
#include <cstdint>

#include "ceSerial.h"
#include "Config.h"

// Serial communication connection parameters
struct SComConnectionParams{
    uint8_t Port;
    uint64_t BaudRate;
    uint8_t DataSize;
    char Parity;
    uint8_t StopBit;
};

union PACKET{
    uint8_t ALL[PACKET_BYTES_VALUE];
    struct{
        uint8_t DATA[DATA_BYTES_VALUE];
        uint8_t ID;
        union{
          uint16_t POST;
          uint8_t POST_8b[2];
        };
    };
};

class SerialCommunication : public SComConnectionParams, public ceSerial
{
private:
    bool successFlag;
    char buffer_char;

public:
    PACKET packet;
    SerialCommunication(SComConnectionParams *sccp) : SComConnectionParams(*sccp){}
    bool init();
    bool receivePacket();
    void transmitPacket(uint8_t* data, uint16_t n);
};

#endif //_COM_PORT_COMMUNICATION_H_
