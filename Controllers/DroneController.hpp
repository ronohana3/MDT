#ifndef DRONE_CONTROLLER_H
#define DRONE_CONTROLLER_H

#include "NavigationController.hpp"
#include "CameraController.hpp"

class DroneController 
{
public:
    DroneController(std::string hostAddress, int navigationPort, int CameraPort);
    ~DroneController();
    void MoveTowardsBox(const cv::Rect box);
    void Takeoff();
    void Land();
    void Scan(const cv::Rect box);
    void GetFrame(cv::Mat &dst);
private:
    NavigationController* m_navigationController;
    CameraController* m_cameraController;

};

#endif