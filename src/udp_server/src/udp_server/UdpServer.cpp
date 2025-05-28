#include <udp_server/UdpServer.hpp>

namespace
{
constexpr std::uint16_t kUdpPort = 8'010;
} // namespace

namespace udp_server
{

UdpServer::UdpServer()
    : m_socket{m_io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), kUdpPort)}
    , m_waited_size{0}
    , m_received_size{0}
    , m_thread{"udp",
               [this]()
               {
                   boost::asio::co_spawn(m_io_context, ReceiveChunk(), boost::asio::detached);
                   m_io_context.run();
               }}
{
}

UdpServer::~UdpServer()
{
    m_io_context.stop();
}

void UdpServer::Subscribe(IFrameChunkConsumer& consumer)
{
    m_chunk_consumer = consumer;
}

boost::asio::awaitable<void> UdpServer::ReceiveChunk()
{
    std::array<std::uint8_t, kChunkSize> buffer{};

    boost::asio::ip::udp::endpoint remote_endpoint;

    co_await m_socket.async_receive_from(boost::asio::buffer(&m_waited_size, sizeof(m_waited_size)),
                                         remote_endpoint,
                                         boost::asio::use_awaitable);

    for (;;)
    {
        if (m_received_size >= m_waited_size)
        {
            co_await m_socket.async_receive_from(boost::asio::buffer(&m_waited_size, sizeof(m_waited_size)),
                                                 remote_endpoint,
                                                 boost::asio::use_awaitable);
            m_received_size = 0;

            assert(m_chunk_consumer && "cnunk consumer must exist");
            m_chunk_consumer->get().OnNewFrame(m_waited_size);
        }

        std::size_t bytes_received = co_await m_socket.async_receive_from(boost::asio::buffer(buffer.data(), buffer.size()),
                                                                          remote_endpoint,
                                                                          boost::asio::use_awaitable);

        assert(m_chunk_consumer && "cnunk consumer must exist");
        m_chunk_consumer->get().OnFrameChunk(buffer, bytes_received);
        m_received_size += bytes_received;
    }
}

} // namespace udp_server
