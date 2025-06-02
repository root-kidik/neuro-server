#include <jpeg_decoder/JpegDecoder.hpp>

namespace jpeg_decoder
{

JpegDecoder::JpegDecoder(IImageConsumer& image_consumer)
    : m_image_consumer{image_consumer}
    , m_is_running{true}
    , m_thread{"Jpeg Decoder",
               [this]()
               {
                   while (m_is_running)
                   {
                       std::vector<std::uint8_t> jpeg;

                       {
                           std::unique_lock lock{m_jpeg_queue_mutex};
                           m_jpeg_queue_cv.wait(lock, [this]() { return !m_jpeg_queue.empty(); });
                           jpeg = std::move(m_jpeg_queue.front());
                           m_jpeg_queue.pop();
                       }

                       auto image = cv::imdecode(cv::Mat(1, static_cast<int>(jpeg.size()), CV_8UC1, jpeg.data()),
                                                 cv::IMREAD_COLOR);

                       m_image_consumer.OnImage(std::move(image));
                   }
               }}

{
}

void JpegDecoder::OnFrame(std::vector<std::uint8_t>&& frame)
{
    {
        std::lock_guard lock{m_jpeg_queue_mutex};
        m_jpeg_queue.push(std::move(frame));
    }

    m_jpeg_queue_cv.notify_one();
}

} // namespace jpeg_decoder
