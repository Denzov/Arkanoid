#include <cstdint>
#include <iostream>
#include <conio.h>
#include "Devices.h"

int main(){
    // for(uint8_t i = 0; i < 18; i++){
    //     pac.ALL[i] = i;
    // }

    // for(uint8_t i = 0; i < 16; i++){
    //     std::cout << (int)pac.DATA[i] << ' ';
    // }

    // std::cout << '\n' << (int)pac.POST << ' ';
    // SCom.init();    
    // while(1){
    //     SCom.receivePacket();
    // }
    uint8_t data[16]{
        0, 1, 2, 3, 4, 5, 6, 7,
        0, 1, 2, 3, 4, 5, 6, 7
    };

    SCom.init();  

    while(1){
        SCom.transmitPacket(data, 8);
        SCom.Delay(2000);
    }
    return 0;
}