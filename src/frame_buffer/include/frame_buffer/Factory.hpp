#pragma once

#include <memory>

#include <frame_buffer/IFrameBuffer.hpp>

namespace frame_buffer
{

std::unique_ptr<IFrameBuffer> MakeFrameBuffer();

}
