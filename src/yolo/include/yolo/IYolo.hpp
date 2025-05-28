#pragma once

#include <yolo/YoloPrediction.hpp>

namespace yolo
{

class IYolo
{
public:
    virtual ~IYolo() = default;

    virtual std::vector<YoloPrediction> Run(const cv::Mat& image) = 0;
};

} // namespace yolo
