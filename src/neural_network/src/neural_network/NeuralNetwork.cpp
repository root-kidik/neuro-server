#include <neural_network/NeuralNetwork.hpp>
#include <yolo/Factory.hpp>

namespace neural_network
{

NeuralNetwork::NeuralNetwork(IDetectionConsumer& detection_consumer)
    : m_detection_consumer{detection_consumer}
    , m_yolo{yolo::MakeYolo({.model_path{"/workspaces/neuro-server/config/yolo11n.onnx"}, .is_rocm_enabled{true}})}
    , m_is_running{true}
    , m_thread{"Neural Network",
               [this]()
               {
                   while (m_is_running)
                   {
                       cv::Mat image;

                       {
                           std::unique_lock lock{m_image_queue_mutex};
                           m_image_queue_cv.wait(lock, [this]() { return !m_image_queue.empty(); });
                           image = std::move(m_image_queue.front());
                           m_image_queue.pop();
                       }

                       auto detections = m_yolo->Run(image);

                       m_detection_consumer.OnDetection(std::move(image), std::move(detections));
                   }
               }}

{
}

void NeuralNetwork::OnImage(cv::Mat&& image)
{
    {
        std::lock_guard lock{m_image_queue_mutex};
        m_image_queue.push(std::move(image));
    }

    m_image_queue_cv.notify_one();
}

} // namespace neural_network
