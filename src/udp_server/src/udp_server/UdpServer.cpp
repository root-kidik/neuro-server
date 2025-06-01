#include <udp_server/UdpServer.hpp>

#include <iostream>

namespace
{
constexpr std::uint16_t kUdpPort = 8'010;
} // namespace

namespace udp_server
{

UdpServer::UdpServer(IDatagramConsumer& datagram_consumer)
    : m_datagram_consumer{datagram_consumer}
    , m_socket{m_io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), kUdpPort)}
    , m_thread{"udp",
               [this]()
               {
                   boost::asio::co_spawn(m_io_context, Receive(), boost::asio::detached);
                   m_io_context.run();
               }}
{
}

UdpServer::~UdpServer()
{
    m_io_context.stop();
}

boost::asio::awaitable<void> UdpServer::Receive()
{
    std::array<std::uint8_t, kMaxDatagramSize> buffer{};
    boost::asio::ip::udp::endpoint             remote_endpoint;

    for (;;)
    {
        std::size_t bytes_received = co_await m_socket.async_receive_from(boost::asio::buffer(buffer),
                                                                          remote_endpoint,
                                                                          boost::asio::use_awaitable);

        m_datagram_consumer.OnDatagram(buffer, bytes_received);
    }
}

} // namespace udp_server