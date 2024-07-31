#include "DroneController.hpp"

DroneController::DroneController(const NavParam navParam, const CamParam camParam,
            std::string droneServerAddress, std::string cameraServerAddress,
            int navigationPort, int cameraPort)
{
    m_navigationController = new NavigationController(navParam, camParam, droneServerAddress, navigationPort);
    m_cameraController = new CameraController(camParam, cameraServerAddress, cameraPort);
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
        isCw = (box.x + box.width/2) > m_camParam.width/2;
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

void DroneController::Stop()
{
    m_navigationController->Stop();
}