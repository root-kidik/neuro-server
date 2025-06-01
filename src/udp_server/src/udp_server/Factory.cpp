#include <udp_server/Factory.hpp>
#include <udp_server/UdpServer.hpp>

namespace udp_server
{

std::unique_ptr<IUdpServer> MakeUdpServer(IFrameChunkConsumer& frame_chunk_consumer)
{
    return std::make_unique<UdpServer>(frame_chunk_consumer);
}

} // namespace udp_server
