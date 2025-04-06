#ifndef _COM_PORT_COMMUNICATION_H_
#define _COM_PORT_COMMUNICATION_H_

#define DEBUG

#include <iostream>
#include <string>
#include <cstdint>

#include "ceSerial.h"
#include "Config.bt.h"

// Serial communication connection parameters
struct SComConnectionParams{
    uint8_t Port;
    uint64_t BaudRate;
    uint8_t DataSize;
    char Parity;
    uint8_t StopBit;
};

enum DATA_FIELDS_STATE : uint8_t{
    LO,
    HI
};

union PACKET{
    uint8_t ALL[PACKET_BYTES_VALUE];
    struct{
        uint8_t PRE[PRE_BYTES_VALUE];
        uint8_t DATA[DATA_BYTES_VALUE];
        
        #if ID_BYTES_VALUE
        uint8_t ID;
        #endif
        
        #if CS_BYTES_VALUE
        uint8_t CS;
        #endif

        uint8_t POST[POST_BYTES_VALUE];
    };
};

class SerialCommunication : public SComConnectionParams, public ceSerial
{
private:
    bool _success_flag;
    uint8_t _buffer_byte;

    uint8_t _CS = 0;
    
    PACKET _transmit_packet;
    PACKET _buffer_packet;

    uint8_t _receive_data[DATA_BYTES_VALUE];

    void print_transmit_packet() const;
    void print_receive_data() const;
    void set_preamble();
    void set_postamble();

public:
    SerialCommunication(SComConnectionParams *sccp) : SComConnectionParams(*sccp){}
    
    bool Init();
    bool ReceivePacket();
    void Transmit(const uint8_t* data);
};

#endif //_COM_PORT_COMMUNICATION_H_
