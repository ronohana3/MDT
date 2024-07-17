#ifndef NAVIGATION_CONTROLLER_H
#define NAVIGATION_CONTROLLER_H

#include <string>
#include <opencv2/opencv.hpp>
#include "CameraController.hpp"
#include "../Utils/SocketClient.hpp"

#define V_Z_SAT 20
#define V_Y_SAT 20
#define W_SAT 1
#define VY_TO_LV(Vy) Vy * 0.0006
#define VZ_TO_RV(Vz) Vz * 0.0005
#define W_TO_LH(W) W * 1

class NavigationController 
{
public:
    NavigationController(std::string hostAddress, int port);
    ~NavigationController();
    void moveTowardsBox(const cv::Rect &box);
    void rotateInPlace(bool isCw);
    void takeoff();
    void land();
private:
    SocketClient socketClient;
    void sendCommand(float Vz, float Vy, float W);
    void sendCommand(std::string command);
};

#endif