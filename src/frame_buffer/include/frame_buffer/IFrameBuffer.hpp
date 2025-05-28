#pragma once

#include <frame_buffer/IFrameConsumer.hpp>
#include <udp_server/IFrameChunkConsumer.hpp>

namespace frame_buffer
{

class IFrameBuffer : public udp_server::IFrameChunkConsumer 
{
public:
    virtual void Subscribe(IFrameConsumer& consumer) = 0;
};

} // namespace frame_buffer
