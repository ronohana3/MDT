#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <opencv2/opencv.hpp>
#include <string>
#include "../Utils/SocketClient.hpp"

#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 720
#define FRAME_AREA FRAME_WIDTH * FRAME_HEIGHT

class CameraController 
{
public:
    CameraController(std::string hostAddress, int port);
    ~CameraController();
    void getFrame(cv::Mat &dst);
private:
SocketClient socketClient;
};

#endif