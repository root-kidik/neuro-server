#include <VideoReceiver.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace
{

constexpr std::uint16_t kUdpPort       = 8'010;
constexpr std::uint16_t kMaxPacketSize = 1'024;
constexpr std::uint32_t kMaxFrameSize  = kMaxPacketSize * 1'000;

} // namespace

namespace neuro_server
{

VideoReceiver::VideoReceiver(boost::asio::io_context& io_context)
    : m_socket{io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), kUdpPort)}
    , m_is_frame_compelted{true}
    , m_waited_size{0}
{
    m_chunks_buffer.reserve(kMaxFrameSize);
    boost::asio::co_spawn(io_context, ReceiveChunk(), boost::asio::detached);
}

boost::asio::awaitable<void> VideoReceiver::ReceiveChunk()
{
    std::array<std::uint8_t, kMaxPacketSize> buffer{};

    boost::asio::ip::udp::endpoint remote_endpoint;

    for (;;)
    {
        if (m_is_frame_compelted)
        {
            co_await m_socket.async_receive_from(boost::asio::buffer(&m_waited_size, sizeof(m_waited_size)),
                                                 remote_endpoint,
                                                 boost::asio::use_awaitable);
            m_is_frame_compelted = false;
            continue;
        }

        std::size_t bytes_received = co_await m_socket.async_receive_from(boost::asio::buffer(buffer.data(), buffer.size()),
                                                                          remote_endpoint,
                                                                          boost::asio::use_awaitable);

        m_chunks_buffer.insert(m_chunks_buffer.end(), buffer.begin(), buffer.begin() + bytes_received);
        if (m_chunks_buffer.size() >= m_waited_size)
        {
            // _consumer.on_jpeg(_buffer);
            m_chunks_buffer.clear();
            m_is_frame_compelted = true;
        }
    }
}

} // namespace neuro_server
