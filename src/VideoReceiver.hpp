#pragma once

#include <boost/asio.hpp>

namespace neuro_server
{

class VideoReceiver final : private boost::asio::noncopyable
{
public:
    explicit VideoReceiver(boost::asio::io_context& io_context);

private:
    boost::asio::awaitable<void> ReceiveChunk();

    boost::asio::ip::udp::socket m_socket;

    bool                      m_is_frame_compelted;
    std::vector<std::uint8_t> m_chunks_buffer;
    std::uint32_t             m_waited_size;
};

} // namespace neuro_server
