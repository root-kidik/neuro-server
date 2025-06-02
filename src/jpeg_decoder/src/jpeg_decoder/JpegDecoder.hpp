#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

#include <jpeg_decoder/IImageConsumer.hpp>
#include <jpeg_decoder/IJpegDecoder.hpp>

#include <boost/noncopyable.hpp>

#include <core/Thread.hpp>

namespace jpeg_decoder
{

class JpegDecoder final
    : public IJpegDecoder
    , private boost::noncopyable
{
public:
    explicit JpegDecoder(IImageConsumer& image_consumer);

private:
    void OnFrame(std::vector<std::uint8_t>&& frame) override;

    IImageConsumer& m_image_consumer;

    std::atomic<bool>       m_is_running;
    std::mutex              m_jpeg_queue_mutex;
    std::condition_variable m_jpeg_queue_cv;

    std::queue<std::vector<std::uint8_t>> m_jpeg_queue;

    core::Thread m_thread;
};

} // namespace jpeg_decoder
