#include "../include/SerialCommunication.h"

bool SerialCommunication::Init(){
    SetPortName("\\\\.\\COM"+std::to_string(Port));
    SetBaudRate(BaudRate);
	SetDataSize(DataSize);
	SetParity(Parity);
	SetStopBits(StopBit);

    set_preamble();
    set_postamble();
    
    if (Open() == 0) {
        #ifdef DEBUG
		std::cout<<"[INFO] Was opening bt port " << Port << '\n';
        #endif
        return 1;
	}
    else {
        #ifdef DEBUG
        std::cout<<"[WARNING] BT PORT WAS NOT OPENING.\n";
        #endif
        return 0;
    }
}

void SerialCommunication::print_transmit_packet() const{
    for(uint16_t i = 0; i < PACKET_BYTES_VALUE; i++){
        std::cout << (int) _transmit_packet.ALL[i] << ' ';
    }
    std::cout << '\n';
}

void SerialCommunication::print_receive_data() const{
    for(uint16_t i = 0; i < DATA_BYTES_VALUE; i++){
        std::cout << (int) _receive_data[i] << ' ';
    }
    std::cout << '\n';
}

void SerialCommunication::set_preamble(){
    *_transmit_packet.PRE = PREAMBLE;
}

void SerialCommunication::set_postamble(){
    *_transmit_packet.POST = POSTAMBLE; 
}

bool SerialCommunication::ReceivePacket(){
    *_buffer_packet.PRE = ReadChar();
    if(*_buffer_packet.PRE == PREAMBLE){
        _CS = 0;
        //> Receive DATA
        for(uint8_t i = 0; i < DATA_BYTES_VALUE; i++){
            _buffer_packet.DATA[i] = ReadChar();            
            _CS ^= _buffer_packet.DATA[i];
        }

        //> Receive CS
        _buffer_packet.CS = ReadChar();
        if(_buffer_packet.CS != _CS){ return false; }

        //> Receive POSTAMBLE
        *_buffer_packet.POST = ReadChar();
        if(*_buffer_packet.POST != POSTAMBLE){ return false; }
        
        for(uint8_t i = 0; i < DATA_BYTES_VALUE; i++){
            _receive_data[i] = _buffer_packet.DATA[i];
        }
    }

    print_receive_data();
    return true;
}


void SerialCommunication::Transmit(const uint8_t* data){
    _CS = 0;
    
    for(uint8_t i = 0; i < DATA_BYTES_VALUE; i++){
       _CS ^= data[i];
       _transmit_packet.DATA[i] = data[i];
    }
    
    _transmit_packet.CS = _CS;
    //print_transmit_packet();
    Write((char*)_transmit_packet.ALL, PACKET_BYTES_VALUE);
}
