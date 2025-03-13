#include "../include/Devices.h"

int main(){
    camera.Init();

    while(1){
        #if DISPLAY
            CAMERA_WORK_IN_MAIN_LOOP();
        #endif
    }

    DETACH_CALC_FRAME();
}