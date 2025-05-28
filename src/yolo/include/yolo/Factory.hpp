#pragma once

#include <yolo/IYolo.hpp>
#include <yolo/YoloInitParams.hpp>

namespace yolo
{

std::shared_ptr<IYolo> MakeYolo(const YoloInitParams& init_params);

} // namespace yolo
