#include "CameraController.hpp"

CameraController::CameraController(int deviceId)
{
    cap = cv::VideoCapture(deviceId);
    if (!cap.isOpened()) {

        std::cout << "cannot open camera with id=" << deviceId << std::endl;
    }
}

CameraController::CameraController(std::string path)
{
    cap = cv::VideoCapture(path);
    if (!cap.isOpened()) {

        std::cout << "cannot open file " << path << std::endl;
    }
}

CameraController::~CameraController()
{
    cap.release();
}

void CameraController::getFrame(cv::Mat &dst)
{
    cap >> dst;
}