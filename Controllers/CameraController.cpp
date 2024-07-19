#include "CameraController.hpp"
#include <iostream>


CameraController::CameraController(std::string hostAddress, int port) : m_socketClient(hostAddress, port),
                                                                m_codecContext(nullptr), m_formatContext(nullptr), 
                                                                m_avFrame(nullptr), m_avPacket(nullptr), m_swsContext(nullptr)
{
    m_socketClient.Open();

    const AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) 
    {
        std::cerr << "H.264 codec not found" << std::endl;
        exit(1);
    }
    m_codecContext = avcodec_alloc_context3(codec);
    if (!m_codecContext) 
    {
        std::cerr << "Could not allocate codec context" << std::endl;
        exit(1);
    }

    if (avcodec_open2(m_codecContext, codec, nullptr) < 0) 
    {
        std::cerr << "Could not open codec" << std::endl;
        exit(1);
    }

    m_avFrame = av_frame_alloc();
    m_avPacket = av_packet_alloc();

    if (!m_avFrame || !m_avPacket) 
    {
        std::cerr << "Could not allocate AVFrame or AVPacket" << std::endl;
        exit(1);
    }
}

CameraController::~CameraController()
{   
    m_socketClient.Close();
    avcodec_free_context(&m_codecContext);
    av_frame_free(&m_avFrame);
    av_packet_free(&m_avPacket);
    sws_freeContext(m_swsContext);
}

int CameraController::GetFrame(cv::Mat &dst)
{
    std::vector<uchar> frameBuffer(FRAME_BUFFER_SIZE);
    int bytesReceived = m_socketClient.Receive((char *)frameBuffer.data(), frameBuffer.size());
    if (bytesReceived <= 0) 
    {
        return false;
    }
    av_packet_unref(m_avPacket);
    m_avPacket->data = frameBuffer.data();
    m_avPacket->size = bytesReceived;

    if (avcodec_send_packet(m_codecContext, m_avPacket) < 0) 
    {
        std::cerr << "Error sending packet to codec context" << std::endl;
        return false;
    }

    if (avcodec_receive_frame(m_codecContext, m_avFrame) == 0)
    {
        int width = m_codecContext->width;
        int height = m_codecContext->height;
        int cvLinesizes[1];
        cvLinesizes[0] = av_image_get_linesize(AV_PIX_FMT_BGR24, width, 0);
        cv::Mat mat(height, width, CV_8UC3);
        uint8_t* cvData[1];
        cvData[0] = mat.data;

        m_swsContext = sws_getCachedContext(m_swsContext,
                                          width, height, m_codecContext->pix_fmt,
                                          width, height, AV_PIX_FMT_BGR24,
                                          SWS_BILINEAR, nullptr, nullptr, nullptr);
        sws_scale(m_swsContext, m_avFrame->data, m_avFrame->linesize, 0, height, cvData, cvLinesizes);
        mat.copyTo(dst);
        mat.release();
        return true;
    }
    return false;
}