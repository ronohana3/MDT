#include <iostream>
#include <vector>
#include <filesystem>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "Tracking/CMT.h"
#include "Detection/inference.h"
#include "Controllers/DroneController.hpp"

using namespace cv;
using namespace std;
using namespace cmt;




#define HOST_ADDRESS "172.16.212.67"
#define NAVIGATION_PORT 10000
#define CAMERA_PORT 9999


void runProgram()
{
    Inference inf("D:\\IronDrone\\MDT\\Detection\\best.onnx", cv::Size(640, 640));

    namedWindow("Stream");
    

    Mat frame, grayFrame;
    Rect boundingBox;
    CMT tracker = CMT();
    bool isTracking = false;
    int i = 1;
    std::cout << "FFmpeg support : " << cv::getBuildInformation().find("FFmpeg") << std::endl;

    DroneController drone(HOST_ADDRESS, NAVIGATION_PORT, CAMERA_PORT);
    // drone.takeoff();
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    // drone.scan(boundingBox);
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    // drone.land();

    while (true) 
    {
        drone.getFrame(frame);

        if(frame.empty())
        {
            std::cout << "Empty Frame" << std::endl;
            continue;
        }

        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        std::cout << "Start proccesing frame #" << i << std::endl;
        std::cout << "Tracking: " << (isTracking ? "yes" : "no" ) << std::endl;

        // Detection
        if (!isTracking)
        {
            std::vector<Detection> output = inf.runInference(frame);
            int detections = output.size();
            std::cout << "Number of detections: " << detections << std::endl;
            if (detections > 0)
            {
                Detection detection = output[0];
                for (int j=1; j<detections; j++)
                    if(output[i].confidence > detection.confidence)
                        detection = output[j];
                if(detection.confidence > 0.6)
                {
                    boundingBox = detection.box;
                    tracker.initialize(grayFrame, boundingBox);
                    isTracking = true;
                    std::cout << "Found drone with confidence=" << detection.confidence << " in boundingBox=" << boundingBox << std::endl;
                }          
            }
        }
        else
        {
            tracker.processFrame(grayFrame);

            if(0.1*tracker.init_points_active_size < tracker.points_active.size() && tracker.points_active.size() > 20)
            {
                Point2f vertices[4];
                tracker.bb_rot.points(vertices);
                for(size_t i = 0; i < tracker.points_active.size(); i++)
                {
                    circle(frame, tracker.points_active[i], 2, Scalar(255,0,0));
                }
                for (int i = 0; i < 4; i++)
                {
                    line(frame, vertices[i], vertices[(i+1)%4], Scalar(255,0,0), 2);
                }

                std::cout << "Active points: " << tracker.points_active.size() << std::endl;
                
                // drone.moveTowardsBox(tracker.bb_rot.boundingRect());
                
            }
            else
            {
                isTracking = false;
                tracker = CMT();

            }
        }

        i++;
        cv::imshow("Stream", frame);

        int key = cv::waitKey(1);
        
        if (key != -1)
        {
            break;
        }
    }

    std::cout << "Finished" << std::endl;

    cv::destroyAllWindows();
}

int main()
{
    runProgram();
    return 0;
}