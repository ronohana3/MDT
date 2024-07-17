#include "CameraController.hpp"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}



CameraController::CameraController(std::string hostAddress, int port) : socketClient(hostAddress, port)
{
    // connect to stream server
    socketClient.open();
}

CameraController::~CameraController()
{   
    // disconnect from stream server
    socketClient.close();
}

void CameraController::getFrame(cv::Mat &dst)
{
    // send a request to server for a frame
    char buffer[100000];
    std::vector<uchar> data;


    int valread = socketClient.recieve(buffer, sizeof(buffer));
    if (valread == 0) {
        return;
    }

    cv::Mat matImg;
    matImg = cv::imdecode(cv::Mat(1, valread, CV_8UC1, buffer), cv::IMREAD_UNCHANGED);
    matImg.copyTo(dst);
}