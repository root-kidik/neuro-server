#pragma once

#include <array>
#include <cstdint>

namespace udp_server
{

inline constexpr std::uint16_t kChunkSize = 1'024;

class IFrameChunkConsumer
{
public:
    virtual ~IFrameChunkConsumer() = default;

    virtual void OnNewFrame(std::uint32_t frame_size)                                                      = 0;
    virtual void OnFrameChunk(const std::array<std::uint8_t, kChunkSize>& chunk, std::uint16_t chunk_size) = 0;
};

} // namespace udp_server
