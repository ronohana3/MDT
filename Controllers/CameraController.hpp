#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <opencv2/opencv.hpp>
#include <string>

class CameraController 
{
public:
    CameraController(int deviceId);
    CameraController(std::string path);
    ~CameraController();
    void getFrame(cv::Mat &dst);
private:
    cv::VideoCapture cap;
};

#endif