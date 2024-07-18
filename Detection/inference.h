#ifndef INFERENCE_H
#define INFERENCE_H

// Cpp native
#include <fstream>
#include <vector>
#include <string>
#include <random>

// OpenCV / DNN / Inference
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

struct Detection
{
    int class_id{0};
    std::string className{};
    float confidence{0.0};
    cv::Rect box{};
};

class Inference
{
public:
    Inference(const std::string &onnxModelPath, const cv::Size &modelInputShape = {640, 640});
    std::vector<Detection> runInference(const cv::Mat &input);

private:
    void LoadOnnxNetwork();
    cv::Mat FormatToSquare(const cv::Mat &source);

    std::string m_modelPath{};

    std::vector<std::string> m_classes{"drone"};

    cv::Size2f m_modelShape{};

    float m_modelConfidenceThreshold {0.25};
    float m_modelScoreThreshold      {0.45};
    float m_modelNMSThreshold        {0.50};

    bool m_letterBoxForSquare = true;

    cv::dnn::Net m_net;
};

#endif // INFERENCE_H
