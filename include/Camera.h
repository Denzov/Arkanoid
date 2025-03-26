#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>

#include "Config.cam.h"
#include "PIreg.h"

enum DEBUG_KEY_STATES{
    REQ_GET_MOUSE_POS = 'k',
    REQ_GET_BALL_POS = 'l',
    REQ_GET_BALL_SPEED = ';',
    REQ_GET_ROBOT_POS = '\'',
};

typedef void (*OnMouse)(int, int, int, int, void*);

struct vec2_float{
    float x, y;
};

struct PointParams{
    vec2_float prev_pos;
    vec2_float pos;
    vec2_float speed;
};

struct CameraConnectionParams{
    void (*onMouse)(int event, int x, int y, int flags, void* userData);
    
    bool* FLAG_GET_MOUSE_POS;

    cv::VideoCapture* cap;

    cv::Scalar _ball_hsv_left_range_begin;
    cv::Scalar _ball_hsv_left_range_end;

    cv::Scalar _ball_hsv_right_range_begin;
    cv::Scalar _ball_hsv_right_range_end;

    cv::Mat _ball_kernel;

    cv::Scalar _robot_hsv_range_begin;
    cv::Scalar _robot_hsv_range_end;

    cv::Mat _robot_kernel;
    
    std::vector<cv::Point2f> _trapezoid_transf;
    std::vector<cv::Point2f> _square_transf;
};

class Camera : private CameraConnectionParams{
private:
    volatile bool FRAMES_IS_READY = 0;

    cv::Mat _cur_frame,
            _hsv_frame,

            _ball_mask,
            _ball_from_hsv_mask_left,
            _ball_from_hsv_mask_right,
            _ball_conc_mask,
            
            _robot_from_hsv_mask,
            _robot_mask,

            _field;

    cv::Mat _mat_transf;

    PointParams _ball{
        .prev_pos = {0, 0},
        .pos = {0, 0},
        .speed = 0
    };
    
    PointParams _robot{
        .prev_pos = {0, 0},
        .pos = {0, 0},
        .speed = 0
    };
    
    float PIXEL_TO_SM;

    float _dt_ms = 0;
    
    std::clock_t _cur_time = 0;
    std::clock_t _prev_time = 0;

    PIreg _pi_reg;

    float _transmitted_u = 0;

    void working_with_frame();
    
    void calc_robot_pos();
    void calc_robot_speed();

    void calc_ball_pos();
    void calc_ball_speed();
    void calc_dt();
    
    void calc_pi();

public:
    Camera(CameraConnectionParams* ccp) : CameraConnectionParams(*ccp){}

    const std::clock_t& CUR_TIME = _cur_time;
    const std::clock_t& PREV_TIME = _prev_time;

    Camera& UpdateTime();

    void Init();
    void DebugPrintsHandler();
    void Tick();
    void ShowWindows();
    
    uint8_t GetU();
    
    void test();
};

#endif // !_CAMERA_H_