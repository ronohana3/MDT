#ifndef DRONE_CONTROLLER_H
#define DRONE_CONTROLLER_H

#include "NavigationController.hpp"
#include "CameraController.hpp"

class DroneController 
{
public:
    DroneController(std::string hostAddress, int navigationPort, int CameraPort);
    ~DroneController();
    void moveTowardsBox(const cv::Rect box);
    void takeoff();
    void land();
    void scan(const cv::Rect box);
    void getFrame(cv::Mat &dst);
private:
    NavigationController* navigationController;
    CameraController* cameraController;

};

#endif