#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace neuro_server
{

class VideoStreamer final : private boost::asio::noncopyable
{
public:
    explicit VideoStreamer(boost::asio::io_context& io_context);

private:
    boost::asio::awaitable<void> Listen();
    boost::asio::awaitable<void> SendFrame(boost::beast::websocket::stream<boost::beast::tcp_stream> ws);

    boost::asio::io_context&       m_io_context;
    boost::asio::ip::tcp::endpoint m_endpoint;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

} // namespace neuro_server
