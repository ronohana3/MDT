#include "CameraController.hpp"
#include <iostream>

CameraController::CameraController(const CamParam& camParam ,const std::string& hostAddress, int port)
    : m_camParam(camParam), m_socketClient(hostAddress, port) {
    m_socketClient.Open();
}

CameraController::~CameraController() {
    m_socketClient.Close();
}

bool CameraController::GetFrame(cv::Mat& dst) {
    m_socketClient.SendPayload("GET_FRAME");

    // Receive the image size first
    uint32_t img_size = 0;
    if (m_socketClient.Receive((char *)&img_size, sizeof(img_size)) != sizeof(img_size)) {
        return false;
    }
    img_size = ntohl(img_size); // Convert to host byte order

    if (img_size == 0) {
        std::cout << "frame size is zero" << std::endl;
        return false;
    }

    // Receive the image data
    std::vector<uchar> img_data(img_size);
    if (m_socketClient.Receive(reinterpret_cast<char*>(img_data.data()), img_size) != img_size) {
        std::cout << "Failed to receive image data" << std::endl;
        return false;
    }

    // Decode the image using OpenCV
    dst = cv::imdecode(img_data, cv::IMREAD_COLOR);
    return !dst.empty();
}
