#pragma once

#include <frame_buffer/Factory.hpp>
#include <udp_server/Factory.hpp>
#include <websocket_server/Factory.hpp>

namespace neuro_server
{

class Service
{
public:
    Service();

private:
    std::unique_ptr<udp_server::IUdpServer>             m_udp_server;
    std::unique_ptr<websocket_server::IWebsocketServer> m_websocket_server;
    std::unique_ptr<frame_buffer::IFrameBuffer>         m_frame_buffer;
};

} // namespace neuro_server
