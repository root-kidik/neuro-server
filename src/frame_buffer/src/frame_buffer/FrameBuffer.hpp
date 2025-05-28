#pragma once

#include <functional>
#include <optional>

#include <frame_buffer/IFrameBuffer.hpp>

#include <boost/noncopyable.hpp>

namespace frame_buffer
{

class FrameBuffer final
    : public IFrameBuffer
    , private boost::noncopyable
{
public:
    FrameBuffer();

    void Subscribe(IFrameConsumer& consumer) override;

    void OnNewFrame(std::uint32_t frame_size) override;
    void OnFrameChunk(const std::array<std::uint8_t, udp_server::kChunkSize>& chunk, std::uint16_t chunk_size) override;

private:
    std::optional<std::reference_wrapper<IFrameConsumer>> m_frame_consumer;

    std::vector<std::uint8_t> m_buffer;

    std::uint32_t m_waited_frame_size;
};

} // namespace frame_buffer