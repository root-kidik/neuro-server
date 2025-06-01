#pragma once

#include <array>
#include <cstdint>

namespace udp_server
{

constexpr std::uint16_t kMaxDatagramSize = 1'500;

class IDatagramConsumer
{
public:
    virtual ~IDatagramConsumer() = default;

    virtual void OnDatagram(const std::array<std::uint8_t, kMaxDatagramSize>& datagram, std::size_t size) = 0;
};

} // namespace udp_server
