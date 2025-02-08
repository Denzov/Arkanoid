#include "SerialCommunication.h"

bool SerialCommunication::init(){
    SetPortName("\\\\.\\COM"+std::to_string(Port));
    SetBaudRate(BaudRate);
	SetDataSize(DataSize);
	SetParity(Parity);
	SetStopBits(StopBit);
	
    packet.POST_8b[0] = 1;
    packet.POST_8b[1] = 2;

    if (Open() == 0) {
        #ifdef DEBUG
		std::cout<<"[INFO] Was opening port " << Port << '\n';
        #endif
        return 1;
	}
    else {
        #ifdef DEBUG
        std::cout<<"[WARNING] PORT WAS NOT OPENING.\n";
        #endif
        return 0;
    }
}

bool SerialCommunication::receivePacket(){
    buffer_char = ReadChar(successFlag);
    
    if (!successFlag) return false;

    while(successFlag)
    {
        std::cout<<buffer_char;
        buffer_char = ReadChar(successFlag);	
    }

    return true;
}

void SerialCommunication::transmitPacket(uint8_t* data, uint16_t n){
    uint16_t data_count = 0;
    
    for(uint16_t pac_data_count = 0;
     pac_data_count < DATA_BYTES_VALUE && data_count < n; pac_data_count++, data_count++){
        packet.DATA[pac_data_count] = data[data_count];
        packet.ID = data_count / DATA_BYTES_VALUE;
        if()Write((char*)packet.ALL, PACKET_BYTES_VALUE);
    }        

    for(uint16_t data_count = 0; data_count < n){
        for(; )

    }
}
    