#include <iostream>
#include <vector>
#include <filesystem>

#include <opencv2/opencv.hpp>

#include "Tracking/CMT.h"
#include "Detection/inference.h"
#include "Navigation/NavigationController.hpp"

using namespace cv;
using namespace std;
using namespace cmt;

int main()
{

    Inference inf("D:\\IronDrone\\MDT\\Detection\\best.onnx", cv::Size(640, 640));

    namedWindow("Stream");
    VideoCapture cap("D:\\IronDrone\\MDT\\assets\\test3.mp4");
    if (!cap.isOpened()) {

        std::cout << "cannot open camera" << std::endl;

    }

    Mat frame, grayFrame;
    Rect boundingBox;
    CMT tracker = CMT();
    bool isTracking = false;
    int i = 1;
    NavigationController navigationController;    
    
    while (true) 
    {
        cap >> frame;
        
        if(frame.empty())
        {
            break;
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
                
                // navigationController.navigateToBox(frame, tracker.bb_rot);
                
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
    
    destroyAllWindows();
    cap.release();

    return 0;
}