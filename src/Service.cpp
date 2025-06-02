#include <Service.hpp>

namespace neuro_server
{

Service::Service()
    : m_websocket_server{websocket_server::MakeWebsocketServer()}
    , m_jpeg_encoder{jpeg_encoder::MakeJpegEncoder(*m_websocket_server)}
    , m_neural_network(neural_network::MakeNeuralNetwork(*m_jpeg_encoder))
    , m_jpeg_decoder{jpeg_decoder::MakeJpegDecoder(*m_neural_network)}
    , m_frame_buffer{frame_buffer::MakeFrameBuffer(*m_jpeg_decoder)}
    , m_udp_server{udp_server::MakeUdpServer(*m_frame_buffer)}
{
}

} // namespace neuro_server
