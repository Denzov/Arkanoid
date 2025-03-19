#include "../include/Camera.h"

void Camera::Init(){
    //INIT WINDOWS
    #if DISPLAY_MAIN_WINDOW
    cv::namedWindow(NAME_MAIN_WINDOW, cv::WINDOW_NORMAL);
    cv::resizeWindow(NAME_MAIN_WINDOW, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
    cv::setMouseCallback(NAME_MAIN_WINDOW, onMouse);
    #endif

    #if DISPLAY_BALL_MASK_WINDOW
    cv::namedWindow(NAME_BALL_MASK_WINDOW, cv::WINDOW_AUTOSIZE);
    cv::resizeWindow(NAME_BALL_MASK_WINDOW, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
    #endif
}

void Camera::DebugPrintsHandler(){
    char CUR_KEY = cv::waitKey(1);
    switch(CUR_KEY){
        case REQ_GET_MOUSE_POS:
            *FLAG_GET_MOUSE_POS = 1;
            break;
        case REQ_GET_BALL_POS:
            std::cout << "ball position x: " << _ball.pos.x << ' ';
            std::cout << "ball position y: " << _ball.pos.y << '\n';
            break;
        case REQ_GET_BALL_SPEED:
            std::cout << "ball speed x: " << _ball.speed.x << ' ';
            std::cout << "ball speed y: " << _ball.speed.y << '\n';
            break;
    }
}

void Camera::working_with_frame(){
    *cap >> _cur_frame;

    cv::cvtColor(_cur_frame, _hsv_frame, cv::COLOR_BGR2HSV);

    cv::inRange(_hsv_frame, _ball_hsv_left_range_begin, _ball_hsv_left_range_end, _ball_from_hsv_mask_left);
    cv::inRange(_hsv_frame, _ball_hsv_right_range_begin, _ball_hsv_right_range_end, _ball_from_hsv_mask_right);

    _ball_conc_mask = _ball_from_hsv_mask_left | _ball_from_hsv_mask_right;

    cv::erode(_ball_conc_mask, _ball_mask, _ball_kernel);

    cv::inRange(_hsv_frame, _robot_hsv_range_begin, _robot_hsv_range_end, _robot_from_hsv_mask);
    cv::erode(_robot_from_hsv_mask, _robot_mask, _robot_kernel);
}

void Camera::calc_robot_pos(){
    float sum_x = 0, sum_y = 0;
    uint16_t value_mask_pixels = 0;

    for (uint16_t y = 0; y < _robot_mask.rows; y++)
    {
        for (uint16_t x = 0; x < _robot_mask.cols; x++)
        {   
            uint8_t cur_pixel = _ball_mask.at<uint8_t>(y, x);
            if(cur_pixel == 255){
                sum_x += x;
                sum_y += y;
                value_mask_pixels++;
            }                
        }
    }

    _robot.pos.x = sum_x / value_mask_pixels;
    _robot.pos.y = sum_y / value_mask_pixels;
}

void Camera::calc_ball_pos(){
    float sum_x = 0, sum_y = 0;
    uint16_t value_mask_pixels = 0;

    for (uint16_t y = 0; y < _ball_mask.rows; y++)
    {
        for (uint16_t x = 0; x < _ball_mask.cols; x++)
        {   
            uint8_t cur_pixel = _ball_mask.at<uint8_t>(y, x);
            if(cur_pixel == 255){
                sum_x += x;
                sum_y += y;
                value_mask_pixels++;
            }                
        }
    }

    _ball.pos.x = sum_x / value_mask_pixels;
    _ball.pos.y = sum_y / value_mask_pixels;
}

Camera& Camera::UpdateTime(){
    _cur_time = std::clock();

    return *this;
}

void Camera::calc_dt(){
    _dt_ms = static_cast<float>(_cur_time - _prev_time) ;
    _prev_time = _cur_time;
}

void Camera::calc_robot_speed(){
    _robot.speed.x = (_robot.pos.x - _robot.prev_pos.x) / _dt_ms;
    _robot.speed.y = (_robot.pos.y - _robot.prev_pos.y) / _dt_ms;

    _robot.prev_pos = _robot.pos;
}

void Camera::calc_ball_speed(){
    _ball.speed.x = (_ball.pos.x - _ball.prev_pos.x) / _dt_ms;
    _ball.speed.y = (_ball.pos.y - _ball.prev_pos.y) / _dt_ms;

    _ball.prev_pos = _ball.pos;
}

void Camera::calc_pi(){
    float ball_pos_in_field = _ball.pos.y - Y_START_FIELD;

    _pi_reg.passSet(ball_pos_in_field);
    _pi_reg.passCur(_robot.pos.y);

    _transmitted_u = _pi_reg.tick()->getU();
}

void Camera::Tick(){
    if(!FRAMES_IS_READY){
        working_with_frame();
        
        calc_robot_pos();
        calc_ball_pos();    
        
        calc_dt();

        calc_ball_speed();
        calc_robot_speed();

        calc_pi();

        FRAMES_IS_READY = 1;
    }
}

void Camera::ShowWindows(){
    #ifdef DISPLAY
        #if DISPLAY_MAIN_WINDOW
            if(FRAMES_IS_READY){                
                cv::imshow(NAME_MAIN_WINDOW, _cur_frame);
            }
        #endif

        #if DISPLAY_BALL_MASK_WINDOW
            if(FRAMES_IS_READY){
                cv::imshow(NAME_BALL_MASK_WINDOW, _ball_mask);
            }
        #endif
    #endif

    FRAMES_IS_READY = 0;
}

uint8_t Camera::GetU(){
    return static_cast<uint8_t>(_transmitted_u);
}

void Camera::test(){
    std::cout << 42 << '\n'; 
}
