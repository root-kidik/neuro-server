#pragma once

#include <frame_buffer/Factory.hpp>
#include <jpeg_decoder/Factory.hpp>
#include <jpeg_encoder/Factory.hpp>
#include <neural_network/Factory.hpp>
#include <udp_server/Factory.hpp>
#include <websocket_server/Factory.hpp>

namespace neuro_server
{

class Service final
{
public:
    Service();

private:
    std::unique_ptr<websocket_server::IWebsocketServer> m_websocket_server;
    std::unique_ptr<jpeg_encoder::IJpegEncoder>         m_jpeg_encoder;
    std::unique_ptr<neural_network::INeuralNetwork>     m_neural_network;
    std::unique_ptr<jpeg_decoder::IJpegDecoder>         m_jpeg_decoder;
    std::unique_ptr<frame_buffer::IFrameBuffer>         m_frame_buffer;
    std::unique_ptr<udp_server::IUdpServer>             m_udp_server;
};

} // namespace neuro_server
