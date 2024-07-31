#include "NavigationController.hpp"
#include <math.h>

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
        float Vz = m_navParam.vzSat*(1 - boxAreaToFrameAreaRatio);
        float Vy = m_navParam.vySat*(1 - (2*boxCenterPixel.y)/m_camParam.height);
        float W = m_navParam.wSat*((2*boxCenterPixel.x)/m_camParam.width - 1);
        SendCommand(Vz, Vy, W);
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

void NavigationController::SendCommand(float Vz, float Vy, float W)
{
    char command[50];
    snprintf(command, sizeof(command), "rc %.2f %.2f %.2f %.2f\r\n", W_TO_LH(W), VY_TO_LV(Vy), 0.0, VZ_TO_RV(Vz));
    SendCommand(command);
}

void NavigationController::SendCommand(std::string command)
{
    std::cout << "NavigationController: Sending command: " << command << std::endl;
    m_socketClient.SendPayload(command.data());
}