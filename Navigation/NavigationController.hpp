#ifndef NAVIGATION_CONTROLLER_H
#define NAVIGATION_CONTROLLER_H

#include <opencv2/opencv.hpp>

class NavigationController 
{
public:
    NavigationController() {}
    void navigateToBox(cv::Mat &frame, cv::RotatedRect &box);

};

#endif