#include <VideoStreamer.hpp>

namespace
{

constexpr std::uint16_t kWebsocketPort = 8'080;

}

namespace neuro_server
{

VideoStreamer::VideoStreamer(boost::asio::io_context& io_context)
    : m_io_context{io_context}
    , m_endpoint{boost::asio::ip::tcp::v4(), kWebsocketPort}
    , m_acceptor{io_context, m_endpoint}
{
    boost::asio::co_spawn(io_context, Listen(), boost::asio::detached);
}

boost::asio::awaitable<void> VideoStreamer::Listen()
{
    for (;;)
    {
        boost::asio::ip::tcp::socket socket = co_await m_acceptor.async_accept(boost::asio::use_awaitable);
        boost::asio::co_spawn(m_io_context,
                              SendFrame(boost::beast::websocket::stream<boost::beast::tcp_stream>(std::move(socket))),
                              boost::asio::detached);
    }
}

boost::asio::awaitable<void> VideoStreamer::SendFrame(boost::beast::websocket::stream<boost::beast::tcp_stream> ws)
{
    co_await ws.async_accept(boost::asio::use_awaitable);

    boost::beast::flat_buffer buffer;

    for (;;)
    {
        co_await ws.async_read(buffer, boost::asio::use_awaitable);
        ws.text(ws.got_text());
        co_await ws.async_write(buffer.data(), boost::asio::use_awaitable);
        buffer.consume(buffer.size());
    }
}

} // namespace neuro_server
