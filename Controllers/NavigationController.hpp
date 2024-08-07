#ifndef NAVIGATION_CONTROLLER_H
#define NAVIGATION_CONTROLLER_H

#include <string>
#include <opencv2/opencv.hpp>
#include "CameraController.hpp"
#include "../Utils/SocketClient.hpp"


typedef struct sNavParam {
    float vySat;
    float vzSat;
    float wSat;
    float vyToLv; //convertion from [cm/s] to controller joystick input [-1,1]
    float vzToRv; //convertion from [cm/s] to controller joystick input [-1,1]
    float wToLh; //convertion from [rad/s] to controller joystick input [-1,1]
    float scanRotationSpeed;

} NavParam;

class NavigationController 
{
public:
    NavigationController(const NavParam& navParam, const CamParam& camParam, std::string hostAddress, int port);
    ~NavigationController();
    void MoveTowardsBox(const cv::Rect &box);
    void RotateInPlace(bool isCw);
    void Takeoff();
    void Land();
    void Stop();
private:
    SocketClient m_socketClient;
    const CamParam m_camParam;
    const NavParam m_navParam;
    cv::Point3f pixelToDirection(const cv::Point2i &pixel);
    void SendCommand(float Vz, float Vy, float W, int timeout = 0);
    void SendCommand(std::string command);
};

#endif