#ifndef _SERIAL_COMMUNICATION_H
#define _SERIAL_COMMUNICATION_H

#include <thread>
#include "Camera.h"
#include "SerialCommunication.h"

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
#if CALC_CAM
void onMouse(int event, int x, int y, int flags, void* userData);

bool FLAG_GET_MOUSE_POS = false;

cv::VideoCapture cap(N_CAPTURE);

cv::Scalar ball_hsv_left_range_begin(BALL_HSV_LEFT_RANGE_BEGIN, BALL_HSV_MIN_LIGHT_LEVEL, BALL_HSV_MIN_LIGHT_LEVEL);
cv::Scalar ball_hsv_left_range_end(BALL_HSV_LEFT_RANGE_END, BALL_HSV_MAX_LIGHT_LEVEL, BALL_HSV_MAX_LIGHT_LEVEL);

cv::Scalar ball_hsv_right_range_begin(BALL_HSV_RIGHT_RANGE_BEGIN, BALL_HSV_MIN_LIGHT_LEVEL, BALL_HSV_MIN_LIGHT_LEVEL);
cv::Scalar ball_hsv_right_range_end(BALL_HSV_RIGHT_RANGE_END, BALL_HSV_MAX_LIGHT_LEVEL, BALL_HSV_MAX_LIGHT_LEVEL);

cv::Mat ball_kernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(BALL_KERNEL_SIZE, BALL_KERNEL_SIZE));


cv::Scalar robot_hsv_range_begin(ROBOT_HSV_RANGE_BEGIN, ROBOT_HSV_MIN_LIGHT_LEVEL, ROBOT_HSV_MIN_LIGHT_LEVEL);
cv::Scalar robot_hsv_range_end(ROBOT_HSV_RANGE_END, ROBOT_HSV_MAX_LIGHT_LEVEL, ROBOT_HSV_MAX_LIGHT_LEVEL);

cv::Mat robot_kernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(ROBOT_KERNEL_SIZE, ROBOT_KERNEL_SIZE));


std::vector<cv::Point2f> square_transf = {
    {0, 0}, {WINDOW_SIZE_WIDTH, 0}, {WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT}, {0, WINDOW_SIZE_HEIGHT}
};

std::vector<cv::Point2f> trapezoid_transf = {
    {X_1_FIELD, Y_1_FIELD}, {X_2_FIELD, Y_2_FIELD}, {X_3_FIELD, Y_3_FIELD}, {X_4_FIELD, Y_4_FIELD}
};

CameraConnectionParams ccp{
    .onMouse = onMouse,

    .FLAG_GET_MOUSE_POS = &FLAG_GET_MOUSE_POS,

    .cap = &cap,

    ._ball_hsv_left_range_begin = ball_hsv_left_range_begin,
    ._ball_hsv_left_range_end = ball_hsv_left_range_end,

    ._ball_hsv_right_range_begin = ball_hsv_right_range_begin,
    ._ball_hsv_right_range_end = ball_hsv_right_range_end,

    ._ball_kernel = ball_kernel,

    ._robot_hsv_range_begin = robot_hsv_range_begin,
    ._robot_hsv_range_end = robot_hsv_range_end,

    ._robot_kernel = robot_kernel,

    ._trapezoid_transf = trapezoid_transf,
    ._square_transf = square_transf
};

Camera camera(&ccp);

void onMouse(int event, int x, int y, int flags, void* userData){
    if(FLAG_GET_MOUSE_POS){
        std::cout << "mouse position x: " << x << '|' << x * SM_TO_PIXEL << ' ';
        std::cout << "mouse position y: " << y << '|' << y * SM_TO_PIXEL << '\n';
        FLAG_GET_MOUSE_POS = 0;
    }
}

void CAMERA_WORK_IN_MAIN_LOOP(){
    camera.ShowWindows();
    camera.DebugPrintsHandler();
    
}
#endif

/*==CALC FRAME FOR ADDED THREAD==*/
#if CALC_CAM
void CALC_FRAME(){
    std::clock_t timer = 0;
    while(1){
        while(camera.UpdateTime().CUR_TIME - timer < Ts_ms)
            ;
        timer = camera.CUR_TIME;

        camera.Tick();
    }
}

std::thread t1(CALC_FRAME);
inline void DETACH_CALC_FRAME(){
    t1.detach();
}
#endif

/*==SENDING ROBOT INFLUENCE==*/
#if CALC_BT 
void SENDING_U(){
    SCom.Init();

    uint8_t sign = 0;
    int16_t u = 0;

    uint8_t data[2];

    std::clock_t timer = 0;

    int i = 0;
    while(1){
        timer = std::clock();

        u = camera.GetU();
        
        if(u < 0){
            u *= -1;
            sign = 1;
        }
        else{
            sign = 0;
        }

        data[0] = static_cast<uint8_t>(u);
        data[1] = static_cast<uint8_t>(sign);
        
        SCom.Transmit(data);
    }
}
    
std::thread t2(SENDING_U);
inline void DETACH_SENDING_U(){
    t2.detach();
}
#endif

#endif // !_SERIAL_COMMUNICATION_H