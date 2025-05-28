#pragma once

#include <string>

#include <opencv4/opencv2/core/types.hpp>

namespace yolo
{

enum class TargetType : std::uint8_t
{
    Helicopter,
    Bird
};

inline std::string TargetTypeToString(TargetType type)
{
    switch (type)
    {
        case TargetType::Helicopter:
            return "helicopter";

        case TargetType::Bird:
            return "bird";

        default:
            assert(false && "unknow target type");
    }

    return "";
}

struct YoloPrediction
{
    float      confidence;
    TargetType target_type;
    cv::Rect   box;
};

} // namespace yolo
