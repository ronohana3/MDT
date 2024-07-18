#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <opencv2/opencv.hpp>
#include <string>
#include "../Utils/SocketClient.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
}

#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 720
#define FRAME_AREA FRAME_WIDTH * FRAME_HEIGHT
#define FRAME_BUFFER_SIZE 100000

class CameraController 
{
public:
    CameraController(std::string hostAddress, int port);
    ~CameraController();
    int GetFrame(cv::Mat &dst);
private:
    SocketClient m_socketClient;
    AVCodecContext* m_codecContext;
    AVFormatContext* m_formatContext;
    AVFrame* m_avFrame;
    AVPacket* m_avPacket;
    SwsContext* m_swsContext;
};

#endif