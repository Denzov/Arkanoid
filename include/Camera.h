#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <atomic>

#include "Config.cam.h"

enum DEBUG_KEY_STATES{
    REQ_GET_MOUSE_POS = 'k',
    REQ_GET_BALL_POS = 'l',
    REQ_GET_BALL_SPEED = ';'
};

typedef void (*OnMouse)(int, int, int, int, void*);

struct CameraConnectionParams{
    void (*onMouse)(int event, int x, int y, int flags, void* userData);
    bool* FLAG_GET_MOUSE_POS;

    cv::VideoCapture* cap;

    cv::Scalar _hsv_left_range_begin;
    cv::Scalar _hsv_left_range_end;

    cv::Scalar _hsv_right_range_begin;
    cv::Scalar _hsv_right_range_end;

    cv::Mat kernel;
};

class Camera : private CameraConnectionParams{
private:
    #if DISPLAY_MAIN_WINDOW
        bool FLAG_DISPLAY_MAIN_WINDOW = DISPLAY_MAIN_WINDOW;
    #endif

    #if DISPLAY_BALL_MASK_WINDOW
        bool FLAG_DISPLAY_BALL_MASK_WINDOW = DISPLAY_BALL_MASK_WINDOW;
    #endif

    volatile bool FRAMES_IS_READY = 0;

    cv::Mat _cur_frame,
            _hsv_frame,
            _ball_mask,
            _from_hsv_ball_mask_left,
            _from_hsv_ball_mask_right,
            _conc_ball_mask;
    
    float _prev_ball_pos_x = 0;
    float _prev_ball_pos_y = 0;

    float _ball_pos_x = 0;
    float _ball_pos_y = 0;

    float _ball_speed_x = 0;
    float _ball_speed_y = 0;
    
public:
    Camera(CameraConnectionParams* ccp) : CameraConnectionParams(*ccp){}

    void Init();
    void DebugPrintsHandler();
    void Tick();
    void ShowWindows();
    void test();
};

#endif // !_CAMERA_H_