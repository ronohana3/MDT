#include "DroneController.hpp"

DroneController::DroneController(std::string hostAddress, int navigationPort, int CameraPort)
{
    m_navigationController = new NavigationController(hostAddress, navigationPort);
    m_cameraController = new CameraController(hostAddress, CameraPort);
}

DroneController::~DroneController() 
{
    delete m_navigationController;
    delete m_cameraController;
}

void DroneController::GetFrame(cv::Mat &dst)
{
    m_cameraController->GetFrame(dst);
}

void DroneController::MoveTowardsBox(const cv::Rect box)
{ 
    m_navigationController->MoveTowardsBox(box);
}

void DroneController::Scan(const cv::Rect box)
{ 
    bool isCw = true;
    if (!box.empty())
        isCw = (box.x + box.width/2) > FRAME_WIDTH/2;
    m_navigationController->RotateInPlace(isCw);
}

void DroneController::Takeoff()
{
    m_navigationController->Takeoff();
}

void DroneController::Land()
{
    m_navigationController->Land();
}