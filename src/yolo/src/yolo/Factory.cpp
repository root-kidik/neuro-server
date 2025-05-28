#include <memory>

#include <yolo/Factory.hpp>
#include <yolo/Yolo.hpp>

namespace yolo
{

std::shared_ptr<IYolo> MakeYolo(const YoloInitParams& init_params)
{
    return std::make_shared<Yolo>(init_params);
}

} // namespace yolo
