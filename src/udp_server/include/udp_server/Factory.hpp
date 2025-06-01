#pragma once

#include <memory>

#include <udp_server/IDatagramConsumer.hpp>
#include <udp_server/IUdpServer.hpp>

namespace udp_server
{

std::unique_ptr<IUdpServer> MakeUdpServer(IDatagramConsumer& datagram_consumer);

}
