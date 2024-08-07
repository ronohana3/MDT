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
    std::cout << "NavigationController: boxAreaToFrameAreaRatio: " << boxAreaToFrameAreaRatio << std::endl;
    
    
        cv::Point3f direction = pixelToDirection(boxCenterPixel);

        float vz = m_navParam.vzSat*(1 - boxAreaToFrameAreaRatio);
        if (boxAreaToFrameAreaRatio > 0.075)
        {
            std::cout << "NavigationController: Box too big stop moving forward" << std::endl;
            vz = 0; 
        }

        float vy = m_navParam.vySat*(1 - (float)(2*boxCenterPixel.y)/m_camParam.height);
        if (std::abs(boxCenterPixel.y - (int)(m_camParam.height)/2) < 10)
        {
            vy = 0;
        }
        
        float wSign = boxCenterPixel.x > m_camParam.cx ? 1 : -1; 
        float yaw = std::acos(direction.z/(std::sqrt(direction.z*direction.z + direction.x*direction.x)));
        float w = (yaw/0.5236) * wSign * m_navParam.wSat;

        if (yaw < 0.0872)
        {
            w = 0;
        }

        SendCommand(vz, vy, w);
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