#pragma once

#include <memory>

#include <udp_server/IUdpServer.hpp>
#include <udp_server/IFrameChunkConsumer.hpp>

namespace udp_server
{

std::unique_ptr<IUdpServer> MakeUdpServer(IFrameChunkConsumer& frame_chunk_consumer);

}
