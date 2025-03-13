#include "../include/Devices.h"

int main(){
    camera.Init();

    while(1){
        CAMERA_WORK_IN_MAIN_LOOP();
    }

    DETACH_CALC_FRAME();
}