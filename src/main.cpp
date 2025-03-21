#include "../include/Devices.h"

int main(){
    #if CALC_CAM
    camera.Init();
    #endif

    while(1){
        #if DISPLAY
            CAMERA_WORK_IN_MAIN_LOOP();
        #endif
    }

    #if CALC_CAM
    DETACH_CALC_FRAME();
    #endif

    #if CALC_BT
    DETACH_SENDING_U();
    #endif
}