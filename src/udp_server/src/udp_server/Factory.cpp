#include <udp_server/Factory.hpp>
#include <udp_server/UdpServer.hpp>

namespace udp_server
{

std::unique_ptr<IUdpServer> MakeUdpServer()
{
    return std::make_unique<UdpServer>();
}

} // namespace udp_server
