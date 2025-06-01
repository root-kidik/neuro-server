#pragma once

#include <mutex>
#include <queue>

#include <frame_buffer/IFrameConsumer.hpp>
#include <websocket_server/IWebsocketServer.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <core/Thread.hpp>

namespace websocket_server
{

class WebsocketServer final
    : public IWebsocketServer
    , private boost::asio::noncopyable
{
public:
    explicit WebsocketServer();
    ~WebsocketServer() override;

    void OnFrame(std::vector<std::uint8_t>&& frame) override;

private:
    boost::asio::awaitable<void> Listen();
    boost::asio::awaitable<void> StreamFrames(boost::beast::websocket::stream<boost::beast::tcp_stream> ws);

    boost::asio::io_context        m_io_context;
    boost::asio::ip::tcp::endpoint m_endpoint;
    boost::asio::ip::tcp::acceptor m_acceptor;

    core::Thread m_thread;

    std::queue<std::vector<std::uint8_t>> m_frames;
    std::condition_variable               m_cv;
    std::mutex                            m_mutex;
};

} // namespace websocket_server
