#pragma once

#include <jpeg_encoder/IJpegConsumer.hpp>

namespace websocket_server
{

class IWebsocketServer : public jpeg_encoder::IJpegConsumer
{
};

} // namespace websocket_server
