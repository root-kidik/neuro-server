#pragma once

#include <udp_server/IFrameChunkConsumer.hpp>

namespace udp_server
{

class IUdpServer
{
public:
    virtual ~IUdpServer() = default;

    virtual void Subscribe(IFrameChunkConsumer& consumer) = 0;
};

} // namespace udp_server
