#include "NavigationController.hpp"
#include <math.h>

NavigationController::NavigationController(std::string hostAddress, int port) : socketClient(hostAddress, port)
{
    socketClient.open();
}

NavigationController::~NavigationController()
{
    socketClient.close();
}


void NavigationController::moveTowardsBox(const cv::Rect &box)
{
    
    cv::Point2i boxCenterPixel = cv::Point2i(box.x + box.width/2, box.y + box.height/2);
    float boxAreaToFrameAreaRatio = (float)box.area()/(FRAME_AREA);
    if (boxAreaToFrameAreaRatio < 0.3)
    {
        float Vz = V_Z_SAT*(1 - boxAreaToFrameAreaRatio);
        float Vy = V_Y_SAT*(1 - (2*boxCenterPixel.y)/FRAME_HEIGHT);
        float W = W_SAT*((2*boxCenterPixel.x)/FRAME_WIDTH - 1);
        sendCommand(Vz, Vy, W);
    }
    else
    {
        sendCommand(0, 0, 0);
    }
}

void NavigationController::rotateInPlace(bool isCw)
{
    float W = 0.5 * W_SAT * (1 ? isCw : -1);
    sendCommand(0, 0, W);
}


void NavigationController::takeoff()
{
    sendCommand("takeoff\r\n");
}

void NavigationController::land()
{
    sendCommand("land\r\n");
}

void NavigationController::sendCommand(float Vz, float Vy, float W)
{
    char command[50];
    snprintf(command, sizeof(command), "rc %.2f %.2f %.2f %.2f\r\n", W_TO_LH(W), VY_TO_LV(Vy), 0.0, VZ_TO_RV(Vz));
    sendCommand(command);
}

void NavigationController::sendCommand(std::string command)
{
    socketClient.sendPayload(command.data());
}