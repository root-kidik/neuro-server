#include <cassert>
#include <cstring>

#include <frame_buffer/FrameBuffer.hpp>

namespace
{
constexpr std::uint8_t kHeaderSize    = 10;
constexpr std::size_t  kMaxPacketSize = kHeaderSize + frame_buffer::kMaxChunkSize;
} // namespace

namespace frame_buffer
{

FrameBuffer::FrameBuffer(IFrameConsumer& consumer)
    : m_frame_consumer{consumer}
    , m_is_running{true}
    , m_thread{"Frame Buffer",
               [this]()
               {
                   while (m_is_running)
                   {
                       std::array<std::uint8_t, udp_server::kMaxDatagramSize> datagram{};
                       std::size_t                                            size{};

                       {
                           std::unique_lock lock{m_datagrams_queue_mutex};
                           m_datagrams_queue_cv.wait(lock, [this]() { return !m_datagrams_queue.empty(); });

                           const auto& [q_datagram, q_size] = m_datagrams_queue.front();
                           datagram                         = q_datagram;
                           size                             = q_size;
                           m_datagrams_queue.pop();
                       }

                       if (size < kHeaderSize)
                       {
                           return;
                       }

                       std::uint32_t frame_id     = *reinterpret_cast<const std::uint32_t*>(datagram.data());
                       std::uint16_t chunk_id     = *reinterpret_cast<const std::uint16_t*>(datagram.data() + 4);
                       std::uint16_t total_chunks = *reinterpret_cast<const std::uint16_t*>(datagram.data() + 6);
                       std::uint16_t chunk_size   = *reinterpret_cast<const std::uint16_t*>(datagram.data() + 8);

                       assert(chunk_size != 0);
                       assert(size == kHeaderSize + chunk_size);
                       assert(chunk_id < total_chunks);
                       assert(chunk_id < kMaxChunks);

                       if (m_frame.frame_id != frame_id)
                       {
                           m_frame          = Frame{};
                           m_frame.frame_id = frame_id;
                       }
                       else
                       {
                           assert(m_frame.total_chunks == total_chunks);
                       }

                       auto& chunk = m_frame.chunks[chunk_id];
                       chunk.size  = chunk_size;

                       std::memcpy(chunk.data.begin(), datagram.begin() + kHeaderSize, chunk_size);

                       m_frame.total_chunks = total_chunks;
                       m_frame.received_chunk_count++;

                       if (m_frame.received_chunk_count == m_frame.total_chunks)
                       {
                           std::vector<std::uint8_t> frame_data;

                           for (const auto& [data, size] : m_frame.chunks)
                           {
                               frame_data.insert(frame_data.end(), data.begin(), data.begin() + size);
                           }

                           m_frame_consumer.OnFrame(std::move(frame_data));
                       }
                   }
               }}
{
}

void FrameBuffer::OnDatagram(const std::array<std::uint8_t, udp_server::kMaxDatagramSize>& datagram, std::size_t size)
{
    {
        std::lock_guard lock{m_datagrams_queue_mutex};
        m_datagrams_queue.emplace(datagram, size);
    }

    m_datagrams_queue_cv.notify_one();
}

} // namespace frame_buffer
