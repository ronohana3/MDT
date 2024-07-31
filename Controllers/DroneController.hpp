#ifndef DRONE_CONTROLLER_H
#define DRONE_CONTROLLER_H

#include "NavigationController.hpp"
#include "CameraController.hpp"

class DroneController 
{
public:
    DroneController(const NavParam navParam, const CamParam camParam, std::string droneServerAddress,std::string cameraServerAddress, int navigationPort, int cameraPort);
    ~DroneController();
    void MoveTowardsBox(const cv::Rect box);
    void Takeoff();
    void Land();
    void Scan(const cv::Rect box);
    void GetFrame(cv::Mat &dst);
    void Stop();
private:
    CamParam m_camParam;
    NavigationController* m_navigationController;
    CameraController* m_cameraController;

};

#endif