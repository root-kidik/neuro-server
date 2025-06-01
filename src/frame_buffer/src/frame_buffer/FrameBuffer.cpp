#include <cassert>

#include <frame_buffer/FrameBuffer.hpp>

namespace
{
constexpr std::uint32_t kFrameBufferInitCapacity = udp_server::kChunkSize * 1'000;
}

namespace frame_buffer
{

FrameBuffer::FrameBuffer() : m_waited_frame_size{0}
{
    m_buffer.reserve(kFrameBufferInitCapacity);
}

void FrameBuffer::Subscribe(IFrameConsumer& consumer)
{
    m_frame_consumers.emplace_back(consumer);
}

void FrameBuffer::OnNewFrame(std::uint32_t frame_size)
{
    m_waited_frame_size = frame_size;
    m_buffer.clear();
}

void FrameBuffer::OnFrameChunk(const std::array<std::uint8_t, udp_server::kChunkSize>& chunk, std::uint16_t chunk_size)
{
    m_buffer.insert(m_buffer.end(), chunk.begin(), chunk.begin() + chunk_size);

    if (m_buffer.size() >= m_waited_frame_size)
    {
        for (auto consumer : m_frame_consumers)
        {
            consumer.get().OnFrame(std::move(m_buffer));
        }

        m_buffer.clear();
    }
}

} // namespace frame_buffer
