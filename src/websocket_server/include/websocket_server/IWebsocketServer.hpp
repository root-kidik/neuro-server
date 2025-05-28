#pragma once

#include <frame_buffer/IFrameBuffer.hpp>

namespace websocket_server
{

class IWebsocketServer : public frame_buffer::IFrameConsumer
{
};

} // namespace websocket_server
