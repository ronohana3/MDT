#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include <opencv2/opencv.hpp>
#include "../Utils/SocketClient.hpp"
#include <string>

typedef struct sCamParam
{
    uint width;
    uint height;
    float fx;
    float fy;
    float cx;
    float cy;
    float f() { return (fx + fy)/2; };
} CamParam;

class CameraController {
public:
    CameraController(const CamParam& camParam, const std::string& hostAddress, int port);
    ~CameraController();

    bool GetFrame(cv::Mat& dst);

private:
    const CamParam m_camParam;
    SocketClient m_socketClient;
};

#endif // CAMERA_CONTROLLER_HPP
