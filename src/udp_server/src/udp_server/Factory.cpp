#include <udp_server/Factory.hpp>
#include <udp_server/UdpServer.hpp>

namespace udp_server
{

std::unique_ptr<IUdpServer> MakeUdpServer(IDatagramConsumer& datagram_consumer)
{
    return std::make_unique<UdpServer>(datagram_consumer);
}

} // namespace udp_server
