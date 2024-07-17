#include "DroneController.hpp"

DroneController::DroneController(std::string hostAddress, int navigationPort, int CameraPort)
{
    navigationController = new NavigationController(hostAddress, navigationPort);
    cameraController = new CameraController(hostAddress, CameraPort);
}

DroneController::~DroneController() 
{
    delete navigationController;
    delete cameraController;
}

void DroneController::getFrame(cv::Mat &dst)
{
    cameraController->getFrame(dst);
}

void DroneController::moveTowardsBox(const cv::Rect box)
{ 
    navigationController->moveTowardsBox(box);
}

void DroneController::scan(const cv::Rect box)
{ 
    bool isCw = true;
    if (!box.empty())
        isCw = (box.x + box.width/2) > FRAME_WIDTH/2;
    navigationController->rotateInPlace(isCw);
}

void DroneController::takeoff()
{
    navigationController->takeoff();
}

void DroneController::land()
{
    navigationController->land();
}