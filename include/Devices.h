#ifndef _SERIAL_COMMUNICATION_H
#define _SERIAL_COMMUNICATION_H

#include <thread>
#include "SerialCommunication.h"
#include "Camera.h"

/*==COMMUNITCATION==*/
SComConnectionParams sccp{
    .Port = BT_PORT,
    .BaudRate = BT_BAUDRATE,
    .DataSize = BT_DATASIZE,
    .Parity = BT_PARITY,    
    .StopBit = BT_STOPBIT
};

SerialCommunication SCom(&sccp);

/*==CAMERA==*/
void onMouse(int event, int x, int y, int flags, void* userData);
bool FLAG_GET_MOUSE_POS = false;

cv::VideoCapture cap(N_CAPTURE);

cv::Scalar hsv_left_range_begin(HSV_LEFT_RANGE_RED_BEGIN, HSV_MIN_LIGHT_LEVEL, HSV_MIN_LIGHT_LEVEL);
cv::Scalar hsv_left_range_end(HSV_LEFT_RANGE_RED_END, HSV_MAX_LIGHT_LEVEL, HSV_MAX_LIGHT_LEVEL);

cv::Scalar hsv_right_range_begin(HSV_RIGHT_RANGE_RED_BEGIN, HSV_MIN_LIGHT_LEVEL, HSV_MIN_LIGHT_LEVEL);
cv::Scalar hsv_right_range_end(HSV_RIGHT_RANGE_RED_END, HSV_MAX_LIGHT_LEVEL, HSV_MAX_LIGHT_LEVEL);

cv::Mat kernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(KERNEL_SIZE, KERNEL_SIZE));

CameraConnectionParams ccp{
    .onMouse = onMouse,
    .FLAG_GET_MOUSE_POS = &FLAG_GET_MOUSE_POS,

    .cap = &cap,

    ._hsv_left_range_begin = hsv_left_range_begin,
    ._hsv_left_range_end = hsv_left_range_end,

    ._hsv_right_range_begin = hsv_right_range_begin,
    ._hsv_right_range_end = hsv_right_range_end,

    .kernel = kernel
};

Camera camera(&ccp);

void onMouse(int event, int x, int y, int flags, void* userData)
{   
    if(FLAG_GET_MOUSE_POS){
        std::cout << "mouse position x: " << x << ' ';
        std::cout << "mouse position y: " << y << '\n';
        FLAG_GET_MOUSE_POS = 0;
    }
}

void CAMERA_WORK_IN_MAIN_LOOP(){
    camera.ShowWindows();
    camera.DebugPrintsHandler();
    
}

/*==CALC FRAME FOR ADDED THREAD==*/
void CALC_FRAME(){
    while(1){
        camera.Tick();
    }
}

std::thread t(CALC_FRAME);
inline void DETACH_CALC_FRAME(){
    t.detach();
}

#endif // !_SERIAL_COMMUNICATION_H