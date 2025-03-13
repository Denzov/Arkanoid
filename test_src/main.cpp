#include <opencv2/opencv.hpp>

#include <thread>
#include <chrono>

using namespace cv;

int main(){
    VideoCapture cap(1);

    namedWindow("frame", cv::WINDOW_AUTOSIZE);

    while(1){
        cv::Mat frame;

        cap >> frame;

        cv::imshow("frame", frame); 
        if(cv::waitKey(1)){}
    }
}