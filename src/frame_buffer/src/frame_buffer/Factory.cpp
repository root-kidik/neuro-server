#include <frame_buffer/Factory.hpp>
#include <frame_buffer/FrameBuffer.hpp>

namespace frame_buffer
{

std::unique_ptr<IFrameBuffer> MakeFrameBuffer()
{
    return std::make_unique<FrameBuffer>();
}

} // namespace frame_buffer
