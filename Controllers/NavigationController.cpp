#include "NavigationController.hpp"
#include <math.h>
#include <chrono>

#define W_TO_LH(w) w * m_navParam.wToLh
#define VZ_TO_RV(vz) vz * m_navParam.vzToRv
#define VY_TO_LV(vy) vy * m_navParam.vyToLv

NavigationController::NavigationController(const NavParam& navParam,const CamParam& camParam,std::string hostAddress, int port) :
    m_navParam(navParam), m_camParam(camParam), m_socketClient(hostAddress, port)
{
    m_socketClient.Open();
}

NavigationController::~NavigationController()
{
    m_socketClient.Close();
}


void NavigationController::MoveTowardsBox(const cv::Rect &box)
{
    
    cv::Point2i boxCenterPixel = cv::Point2i(box.x + box.width/2, box.y + box.height/2);
    float boxAreaToFrameAreaRatio = (float)box.area()/(m_camParam.height*m_camParam.width);
    if (boxAreaToFrameAreaRatio < 0.3)
    {
        float vz = m_navParam.vzSat*(1 - boxAreaToFrameAreaRatio);
        float vy = m_navParam.vySat*(1 - (2*boxCenterPixel.y)/m_camParam.height);
        // float w = m_navParam.wSat*((2*boxCenterPixel.x)/m_camParam.width - 1);
        float wSign = boxCenterPixel.x > m_camParam.cx ? 1 : -1; 
        cv::Point3f velocity = cv::Point3f(vy, vz, wSign*m_navParam.wSat);

        cv::Point3f direction = pixelToDirection(boxCenterPixel);

        moveAlongDirection(direction, velocity);

    }
    else
    {
        std::cout << "NavigationController: Box too big stopping" << std::endl;
        SendCommand(0, 0, 0);
    }
}

void NavigationController::RotateInPlace(bool isCw)
{
    float w = m_navParam.scanRotationSpeed * (1 ? isCw : -1);
    SendCommand(0, 0, w);
}


cv::Point3f NavigationController::pixelToDirection(const cv::Point2i &pixel)
{
    double f = (double)((m_camParam.fx + m_camParam.fy) / 2.0);
    double nx = (pixel.x - m_camParam.cx) / f;
    double ny = (pixel.y - m_camParam.cy) / f;
    cv::Point3f n = cv::Point3f(nx, ny, 1) / cv::norm(cv::Point3f(nx, ny, 1));
    return n;
}

void NavigationController::moveAlongDirection(const cv::Point3f &direction, const cv::Point3f &velocity)
{
    float angle = std::acos(direction.z/(std::sqrt(direction.z*direction.z + direction.x*direction.x)));
    std::cout << "Angle: " << angle << std::endl;
    float rotationVelocity = velocity.z; 
    int timeToRotate = int((angle / abs(rotationVelocity))*1000); // in seconds
    std::cout << "timeToRotate: " << timeToRotate << std::endl;
    // SendCommand(VY_TO_LV(velocity[0]), VZ_TO_RV(velocity[1]), W_TO_LH(rotationVelocity));
    if (angle > 0.0872)
    {
        SendCommand(0, 0, W_TO_LH(rotationVelocity), timeToRotate);
    }
    else
    {
        // SendCommand(0, 0, 0, timeToRotate);
    }
    
}

void NavigationController::Takeoff()
{
    SendCommand("takeoff\r\n");
}

void NavigationController::Land()
{
    SendCommand("land\r\n");
}

void NavigationController::Stop()
{
    SendCommand(0,0,0);
}

void NavigationController::SendCommand(float vz, float vy, float w, int timeout)
{
    // timeout in milliseconds
    char command[50];
    if (timeout > 0)
    {
        snprintf(command, sizeof(command), "rc %.2f %.2f %.2f %.2f %d\r\n", W_TO_LH(w), VY_TO_LV(vy), 0.0, VZ_TO_RV(vz), timeout);
        // wait for timeout to prevent two commands race conditions
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    }
    else
    {
        snprintf(command, sizeof(command), "rc %.2f %.2f %.2f %.2f\r\n", W_TO_LH(w), VY_TO_LV(vy), 0.0, VZ_TO_RV(vz));
    }
    SendCommand(command);
}

void NavigationController::SendCommand(std::string command)
{
    std::cout << "NavigationController: Sending command: " << command << std::endl;
    m_socketClient.SendPayload(command.data());
}