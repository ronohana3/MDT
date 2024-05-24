#include "DroneController.hpp"

DroneController::DroneController(std::string cameraStreamPath) : camera(cameraStreamPath) {}

DroneController::DroneController(int cameraId) : camera(cameraId) {}

DroneController::~DroneController() {}

void DroneController::getFrame(cv::Mat &dst)
{
    camera.getFrame(dst);
}

void DroneController::navigateToBox(const cv::Mat &frame, const cv::RotatedRect &box)
{

}