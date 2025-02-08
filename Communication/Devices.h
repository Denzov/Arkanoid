#include "SerialCommunication.h"

SComConnectionParams sccp{
    .Port = BT_PORT,
    .BaudRate = BT_BAUDRATE,
    .DataSize = BT_DATASIZE,
    .Parity = BT_PARITY,    
    .StopBit = BT_STOPBIT
};


SerialCommunication SCom(&sccp);

