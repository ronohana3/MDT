#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace cv;
using namespace std;

#define MAX_FRAMES = 1000
#define LEARNING_RATE = -1   

vector<Point2f> toPoints(const vector<KeyPoint>& keyPoints)
{
    vector<Point2f> points;
    for (auto& keyPoint : keyPoints)
    {
        points.push_back(keyPoint.pt);
    }
    return points;
}

int main()
{
    
    Ptr<BackgroundSubtractorMOG2> fgbg = createBackgroundSubtractorMOG2();
    Mat frame;


    // Mat frame, currFrame, prevFrame;
    // vector<KeyPoint> prevFrameKeyPoints;
    // vector<Point2f> prevFramePoints;
    
    // Ptr<FastFeatureDetector> detector = FastFeatureDetector::create();
    
    namedWindow("Stream");

    VideoCapture cap(0);
    // VideoCapture cap("../../IMG_0921.MOV");
    if (!cap.isOpened()) {

        cout << "cannot open camera";

    }

    // cap >> prevFrame;
    // resize(prevFrame, prevFrame, Size(), 0.3,0.3);
    // cvtColor(prevFrame, prevFrame, COLOR_BGR2GRAY);

    // detector->detect(prevFrame, prevFrameKeyPoints);
    // prevFramePoints = toPoints(prevFrameKeyPoints);

    while (true) 
    {

        cap >> frame;
        
        if(frame.empty())
            break;

        // resize(frame, frame, Size(), 0.3,0.3);
        
        // cvtColor(frame, currFrame, COLOR_BGR2GRAY);


        // vector<float> err;
        // vector<unsigned char> status;
        // vector<Point2f> currFramePoints;

        // calcOpticalFlowPyrLK(prevFrame, currFrame, prevFramePoints, currFramePoints, status, err, Size(21,21), 3, cv::TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 30, (0.01)), OPTFLOW_LK_GET_MIN_EIGENVALS);

        
        // vector<float> distances;
        // vector<Point2f> prevFrameGoodPoints;
        // vector<Point2f> currFrameGoodPoints;
        // for (int i=0; i<prevFramePoints.size(); i++)
        // {
            
        //     if (status[i] == 1 && err[i] < 1)
        //     {
        //         float distance = norm(prevFramePoints[i] - currFramePoints[i]);
        //         prevFrameGoodPoints.push_back(prevFramePoints[i]);
        //         currFrameGoodPoints.push_back(currFramePoints[i]);
        //         distances.push_back(distance);
        //         // cout << "err: " << err[i] << " status: " << (int)status[i]  << " distance: " << distance << endl;
        //     }
        // }

        // Mat distancesMat(distances);
        // distancesMat = distancesMat.reshape(1);

        // Mat labels, centers;
        // kmeans(distancesMat, 2, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3, KMEANS_PP_CENTERS, centers);

        // map<int, vector<Point2f>> clusteredPoints;
        // for (size_t i = 0; i < prevFrameGoodPoints.size(); ++i) {
        //     int label = labels.at<int>(i);
        //     clusteredPoints[label].push_back(currFrameGoodPoints[i]);
        // }

        // Scalar color = Scalar(255, 0, 0); // Blue color for the second cluster

        // int smallerClusterSize = INT_MAX;
        // int smallerClusterLabel;

        // // Find the smaller cluster
        // for (const auto& cluster : clusteredPoints) {
        //     if (cluster.second.size() < smallerClusterSize) {
        //         smallerClusterSize = cluster.second.size();
        //         smallerClusterLabel = cluster.first;
        //     }
        // }

        // for (const auto& cluster : clusteredPoints) {
        //     if (cluster.first == smallerClusterLabel) {
        //         for (const auto& point : cluster.second) {
        //             circle(frame, point, 3, color, -1); // Draw filled circle for the smaller cluster
        //         }
        //     }
        // }


        fgbg->clear();

        Mat mask;

        fgbg->apply(frame,mask);

        vector<vector<Point>> contours;

        threshold(mask, mask, 200, 255, THRESH_BINARY);

        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3,3));
        morphologyEx(mask, mask, MORPH_OPEN, kernel);
        
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (auto &contour : contours)
        {
            if (contourArea(contour) > 0.0005*frame.size().area())
                rectangle(frame, boundingRect(contour), Scalar(0, 255, 0), 8);
        }

        // prevFrame = currFrame.clone();
        // detector->detect(prevFrame, prevFrameKeyPoints);
        // prevFramePoints = toPoints(prevFrameKeyPoints);
        
        imshow("Stream", frame);

        int key = waitKey(1);
        
        if (key != -1)
        {
            break;
        }
    }

    return 0;
}