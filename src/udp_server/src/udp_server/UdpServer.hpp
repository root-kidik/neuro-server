#pragma once

#include <udp_server/IDatagramConsumer.hpp>
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
    explicit UdpServer(IDatagramConsumer& datagram_consumer);
    ~UdpServer() override;

private:
    boost::asio::awaitable<void> Receive();

    IDatagramConsumer& m_datagram_consumer;

    boost::asio::io_context m_io_context;

    boost::asio::ip::udp::socket m_socket;

    core::Thread m_thread;
};

} // namespace udp_server
