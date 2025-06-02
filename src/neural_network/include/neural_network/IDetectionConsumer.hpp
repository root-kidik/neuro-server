#pragma once

#include <opencv4/opencv2/opencv.hpp>
#include <yolo/YoloPrediction.hpp>

namespace neural_network
{

class IDetectionConsumer
{
public:
    virtual ~IDetectionConsumer() = default;

    virtual void OnDetection(cv::Mat&& image, std::vector<yolo::YoloPrediction>&& predictions) = 0;
};

} // namespace neural_network
