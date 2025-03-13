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
            std::cout << "ball position x: " << _ball_pos_x << ' ';
            std::cout << "ball position y: " << _ball_pos_y << '\n';
            break;
        case REQ_GET_BALL_SPEED:
            std::cout << "ball speed x: " << _ball_speed_x << ' ';
            std::cout << "ball speed y: " << _ball_speed_y << '\n';
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

    _ball_pos_x = sum_x / value_mask_pixels;
    _ball_pos_y = sum_y / value_mask_pixels;
}

Camera& Camera::UpdateTime(){
    _cur_time = std::clock();

    return *this;
}

void Camera::calc_dt(){
    _dt_ms = static_cast<float>(_cur_time - _prev_time) ;
    _prev_time = _cur_time;
}

void Camera::calc_ball_speed(){
    _ball_speed_x = (_ball_pos_x - _prev_ball_pos_x) / _dt_ms;
    _ball_speed_y = (_ball_pos_y - _prev_ball_pos_y) / _dt_ms;
}

void Camera::Tick(){
    if(!FRAMES_IS_READY){
        working_with_frame();

        calc_ball_pos();    
        calc_dt();
        calc_ball_speed();

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

void Camera::test(){
    std::cout << 8 << '\n'; 
}
