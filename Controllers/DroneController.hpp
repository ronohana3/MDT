#ifndef DRONE_CONTROLLER_H
#define DRONE_CONTROLLER_H

#include "NavigationController.hpp"
#include "CameraController.hpp"

class DroneController 
{
public:
    DroneController(std::string cameraStreamPath);
    DroneController(int cameraId);
    ~DroneController();
    void navigateToBox(const cv::Mat &frame, const cv::RotatedRect &box);
    void getFrame(cv::Mat &dst);
private:
    NavigationController navigationController;
    CameraController camera;

};

#endif