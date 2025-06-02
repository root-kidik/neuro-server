#include <jpeg_encoder/JpegEncoder.hpp>

namespace
{
constexpr int    kJpegQuality = 85;
const cv::Scalar kDetectionColor{0, 128, 0};
} // namespace

namespace jpeg_encoder
{

JpegEncoder::JpegEncoder(IJpegConsumer& jpeg_consumer)
    : m_jpeg_consumer{jpeg_consumer}
    , m_is_running{true}
    , m_thread{"Jpeg Encoder",
               [this]()
               {
                   while (m_is_running)
                   {
                       cv::Mat                           image;
                       std::vector<yolo::YoloPrediction> detections;

                       {
                           std::unique_lock lock{m_image_queue_mutex};
                           m_image_queue_cv.wait(lock, [this]() { return !m_image_queue.empty(); });
                           auto& [q_image, q_detections] = m_image_queue.front();
                           image                         = std::move(q_image);
                           detections                    = std::move(q_detections);
                           m_image_queue.pop();
                       }

                       for (auto& detection : detections)
                       {
                           cv::rectangle(image, detection.box, kDetectionColor, 3);

                           auto confidence = std::floor(100 * detection.confidence) / 100;

                           auto label = yolo::TargetTypeToString(detection.target_type) + " " +
                                        std::to_string(confidence).substr(0, std::to_string(confidence).size() - 4);

                           cv::rectangle(image,
                                         cv::Point{detection.box.x, detection.box.y - 25},
                                         cv::Point{detection.box.x + static_cast<int>(label.length()) * 15,
                                                   detection.box.y},
                                         kDetectionColor,
                                         cv::FILLED);

                           cv::putText(image,
                                       label,
                                       cv::Point(detection.box.x, detection.box.y - 5),
                                       cv::FONT_HERSHEY_SIMPLEX,
                                       0.75,
                                       cv::Scalar(0, 0, 0),
                                       2);
                       }

                       std::vector<std::uint8_t> jpeg;
                       std::vector<int>          params = {cv::IMWRITE_JPEG_QUALITY,
                                                           kJpegQuality,
                                                           cv::IMWRITE_JPEG_PROGRESSIVE,
                                                           1,
                                                           cv::IMWRITE_JPEG_OPTIMIZE,
                                                           1};
                       cv::imencode(".jpg", image, jpeg, params);

                       m_jpeg_consumer.OnJpeg(std::move(jpeg));
                   }
               }}

{
}

void JpegEncoder::OnDetection(cv::Mat&& image, std::vector<yolo::YoloPrediction>&& predictions)
{
    {
        std::lock_guard lock{m_image_queue_mutex};
        m_image_queue.emplace(std::move(image), std::move(predictions));
    }

    m_image_queue_cv.notify_one();
}

} // namespace jpeg_encoder
