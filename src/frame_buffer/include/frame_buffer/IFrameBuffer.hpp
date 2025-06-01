#pragma once

#include <frame_buffer/IFrameConsumer.hpp>
#include <udp_server/IDatagramConsumer.hpp>

namespace frame_buffer
{

class IFrameBuffer : public udp_server::IDatagramConsumer
{
};

} // namespace frame_buffer
