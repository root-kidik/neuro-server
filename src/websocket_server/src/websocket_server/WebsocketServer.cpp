#include <websocket_server/WebsocketServer.hpp>

namespace
{
constexpr std::uint16_t kWebsocketPort = 8'080;
}

namespace websocket_server
{

WebsocketServer::WebsocketServer()
    : m_endpoint{boost::asio::ip::tcp::v4(), kWebsocketPort}
    , m_acceptor{m_io_context, m_endpoint}
    , m_thread("ws",
               [this]()
               {
                   boost::asio::co_spawn(m_io_context, Listen(), boost::asio::detached);
                   m_io_context.run();
               })
{
}

WebsocketServer::~WebsocketServer()
{
    m_io_context.stop();
}

void WebsocketServer::OnJpeg(std::vector<std::uint8_t>&& jpeg)
{
    {
        std::lock_guard lock{m_mutex};
        m_jpegs.push(std::move(jpeg));
    }

    m_cv.notify_one();
}

boost::asio::awaitable<void> WebsocketServer::Listen()
{
    for (;;)
    {
        boost::asio::ip::tcp::socket socket = co_await m_acceptor.async_accept(boost::asio::use_awaitable);
        boost::asio::co_spawn(m_io_context,
                              StreamFrames(boost::beast::websocket::stream<boost::beast::tcp_stream>(std::move(socket))),
                              boost::asio::detached);
    }
}

boost::asio::awaitable<void> WebsocketServer::StreamFrames(boost::beast::websocket::stream<boost::beast::tcp_stream> ws)
{
    co_await ws.async_accept(boost::asio::use_awaitable);

    for (;;)
    {
        std::vector<std::uint8_t> frame;
        {
            std::unique_lock lock{m_mutex};
            m_cv.wait(lock, [this]() { return !m_jpegs.empty(); });

            frame = std::move(m_jpegs.front());
            m_jpegs.pop();
        }

        ws.binary(true);
        co_await ws.async_write(boost::asio::buffer(frame), boost::asio::use_awaitable);
    }
}

} // namespace websocket_server
