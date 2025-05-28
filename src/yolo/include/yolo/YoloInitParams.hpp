#pragma once

#include <string>

#include <opencv4/opencv2/core/types.hpp>

namespace yolo
{

struct YoloInitParams
{
    std::string model_path;

    float rectangle_confidence_threshold = 0.1F;
    float non_max_suppression_threshold  = 0.5F;
    bool  is_cuda_enabled                = false;
    bool  is_half_enabled                = false;
};

} // namespace yolo
