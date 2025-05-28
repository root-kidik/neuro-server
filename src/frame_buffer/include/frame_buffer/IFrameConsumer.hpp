#pragma once

#include <cstdint>
#include <vector>

namespace frame_buffer
{

class IFrameConsumer
{
public:
    virtual ~IFrameConsumer() = default;

    virtual void OnFrame(std::vector<std::uint8_t>&& frame) = 0;
};

} // namespace frame_buffer
