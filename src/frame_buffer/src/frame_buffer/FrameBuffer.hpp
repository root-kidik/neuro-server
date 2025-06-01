#pragma once

#include <array>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <optional>
#include <queue>

#include <frame_buffer/IFrameBuffer.hpp>

#include <boost/noncopyable.hpp>

#include <core/Thread.hpp>

namespace frame_buffer
{

inline constexpr std::uint16_t kMaxChunks    = 256;
inline constexpr std::uint16_t kMaxChunkSize = 1'400;

struct Chunk
{
    std::array<std::uint8_t, kMaxChunkSize> data{};
    std::uint32_t                           size{};
};

struct Frame
{
    std::uint32_t frame_id{};
    std::uint16_t total_chunks{};
    std::uint16_t received_chunk_count{};

    std::array<Chunk, kMaxChunks> chunks{};
};

class FrameBuffer final
    : public IFrameBuffer
    , private boost::noncopyable
{
public:
    explicit FrameBuffer(IFrameConsumer& consumer);

    void OnDatagram(const std::array<std::uint8_t, udp_server::kMaxDatagramSize>& datagram, std::size_t size) override;

private:
    void ProcessDatagram();

    IFrameConsumer& m_frame_consumer;
    Frame           m_frame;

    std::atomic<bool>       m_is_running;
    std::mutex              m_datagrams_queue_mutex;
    std::condition_variable m_datagrams_queue_cv;

    std::queue<std::pair<std::array<std::uint8_t, udp_server::kMaxDatagramSize>, std::size_t>> m_datagrams_queue;

    core::Thread m_thread;
};

} // namespace frame_buffer
