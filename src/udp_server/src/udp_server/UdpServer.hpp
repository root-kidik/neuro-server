#pragma once

#include <udp_server/IFrameChunkConsumer.hpp>
#include <udp_server/IUdpServer.hpp>

#include <boost/asio.hpp>

#include <core/Thread.hpp>

namespace udp_server
{

class UdpServer final
    : public IUdpServer
    , private boost::asio::noncopyable
{
public:
    explicit UdpServer(IFrameChunkConsumer& chunk_consumer);
    ~UdpServer() override;

private:
    boost::asio::awaitable<void> ReceiveChunk();

    IFrameChunkConsumer& m_chunk_consumer;

    boost::asio::io_context m_io_context;

    boost::asio::ip::udp::socket m_socket;

    std::uint32_t m_waited_size;
    std::uint32_t m_received_size;

    core::Thread m_thread;
};

} // namespace udp_server
