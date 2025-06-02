#include <Service.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace neuro_server
{

Service::Service()
    : m_frame_buffer{frame_buffer::MakeFrameBuffer(*this)}
    , m_udp_server{udp_server::MakeUdpServer(*m_frame_buffer)}
    , m_websocket_server{websocket_server::MakeWebsocketServer()}
    , m_yolo{yolo::MakeYolo({.model_path{"/workspaces/neuro-server/config/yolo11n.onnx"}, .is_rocm_enabled{true}})}
    , m_is_runned{true}
    , m_thread{"Service",
               [this]()
               {
                   const cv::Scalar color{0, 128, 0};

                   while (m_is_runned)
                   {
                       std::vector<std::uint8_t> frame;
                       {
                           std::unique_lock lock{m_mutex};
                           m_cv.wait(lock, [this]() { return !m_frames_queue.empty(); });
                           frame = std::move(m_frames_queue.front());
                           m_frames_queue.pop();
                       }

                       auto image = cv::imdecode(cv::Mat(1, static_cast<int>(frame.size()), CV_8UC1, frame.data()),
                                                 cv::IMREAD_COLOR);

                       if (image.empty())
                       {
                           continue;
                       }

                       auto detections = m_yolo->Run(image);

                       for (auto& detection : detections)
                       {
                           cv::rectangle(image, detection.box, color, 3);

                           auto confidence = std::floor(100 * detection.confidence) / 100;

                           auto label = yolo::TargetTypeToString(detection.target_type) + " " +
                                        std::to_string(confidence).substr(0, std::to_string(confidence).size() - 4);

                           cv::rectangle(image,
                                         cv::Point{detection.box.x, detection.box.y - 25},
                                         cv::Point{detection.box.x + static_cast<int>(label.length()) * 15,
                                                   detection.box.y},
                                         color,
                                         cv::FILLED);

                           cv::putText(image,
                                       label,
                                       cv::Point(detection.box.x, detection.box.y - 5),
                                       cv::FONT_HERSHEY_SIMPLEX,
                                       0.75,
                                       cv::Scalar(0, 0, 0),
                                       2);
                       }

                       std::vector<std::uint8_t> jpeg_buffer;
                       std::vector<int>          params =
                           {cv::IMWRITE_JPEG_QUALITY, 85, cv::IMWRITE_JPEG_PROGRESSIVE, 1, cv::IMWRITE_JPEG_OPTIMIZE, 1};
                       cv::imencode(".jpg", image, jpeg_buffer, params);

                       m_websocket_server->OnFrame(std::move(jpeg_buffer));
                   }
               }}
{
}

Service::~Service()
{
    m_is_runned = false;
}

void Service::OnFrame(std::vector<std::uint8_t>&& frame)
{
    {
        std::lock_guard lock{m_mutex};
        m_frames_queue.push(std::move(frame));
    }

    m_cv.notify_one();
}

} // namespace neuro_server
