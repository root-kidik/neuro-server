#include <Service.hpp>

namespace neuro_server
{

Service::Service()
    : m_udp_server{udp_server::MakeUdpServer()}
    , m_websocket_server{websocket_server::MakeWebsocketServer()}
    , m_frame_buffer{frame_buffer::MakeFrameBuffer()}
{
    m_udp_server->Subscribe(*m_frame_buffer);
    m_frame_buffer->Subscribe(*m_websocket_server);
}

} // namespace neuro_server
