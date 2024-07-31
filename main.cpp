#include <iostream>
#include <vector>
#include <filesystem>
#include <chrono>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

#include "Tracking/CMT.h"
#include "Detection/inference.h"
#include "Controllers/DroneController.hpp"

void runProgram()
{
    std::ifstream fSettings("D:\\IronDrone\\MDT\\ApplicationSettings.json");
    nlohmann::json data = nlohmann::json::parse(fSettings);
    fSettings.close();
    

    const CamParam camParam {
        data["Camera"]["Parameters"]["width"],
        data["Camera"]["Parameters"]["height"],
        data["Camera"]["Parameters"]["fx"],
        data["Camera"]["Parameters"]["fy"],
        data["Camera"]["Parameters"]["cx"],
        data["Camera"]["Parameters"]["cy"]
    };


    const NavParam navParam {
        data["Navigation"]["Velocities"]["vySat"],
        data["Navigation"]["Velocities"]["vzSat"],
        data["Navigation"]["Velocities"]["wSat"],
        data["Navigation"]["Velocities"]["vyToLv"],
        data["Navigation"]["Velocities"]["vzToRv"],
        data["Navigation"]["Velocities"]["wToLh"],
        data["Navigation"]["Velocities"]["scanRotationSpeed"]
    };


    Inference inf(data["onnxPath"], cv::Size(640, 640));

    cv::namedWindow("Stream");
    

    cv::Mat frame, grayFrame;
    cv::Rect boundingBox;
    cmt::CMT tracker = cmt::CMT();
    bool isTracking = false;
    int i = 1;

    DroneController drone(navParam, camParam,
                    data["Navigation"]["serverAddress"], 
                    data["Camera"]["serverAddress"], 
                    data["Navigation"]["serverPort"],
                    data["Camera"]["serverPort"]);

    drone.Takeoff();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    while (true) 
    {
        drone.GetFrame(frame);

        if(frame.empty())
        {
            std::cout << "Empty Frame" << std::endl;
            continue;
        }

        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        std::cout << "Start proccesing frame " << frame.size() << " #" << i << std::endl;
        std::cout << "Tracking: " << (isTracking ? "yes" : "no" ) << std::endl;

        // Detection
        if (!isTracking)
        {
            // std::cout << "Start Scan" << std::endl;
            // drone.Scan(boundingBox);

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
                    cv::circle(frame, tracker.points_active[i], 2, cv::Scalar(255,0,0));
                }
                for (int i = 0; i < 4; i++)
                {
                    cv::line(frame, vertices[i], vertices[(i+1)%4], cv::Scalar(255,0,0), 2);
                }

                std::cout << "Active points: " << tracker.points_active.size() << std::endl;
                
                drone.MoveTowardsBox(tracker.bb_rot.boundingRect());
                
            }
            else
            {
                isTracking = false;
                tracker = cmt::CMT();

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

    drone.Stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    drone.Land();
    std::cout << "Finished" << std::endl;
    cv::destroyAllWindows();
}

int main()
{
    runProgram();
    return 0;
}