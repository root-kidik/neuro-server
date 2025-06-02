#pragma once

#include <opencv4/opencv2/opencv.hpp>

namespace jpeg_decoder
{

class IImageConsumer
{
public:
    virtual ~IImageConsumer() = default;

    virtual void OnImage(cv::Mat&& image) = 0;
};

} // namespace jpeg_decoder
