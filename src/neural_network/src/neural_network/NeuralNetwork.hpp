#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

#include <neural_network/IDetectionConsumer.hpp>
#include <neural_network/INeuralNetwork.hpp>
#include <yolo/IYolo.hpp>

#include <boost/noncopyable.hpp>

#include <core/Thread.hpp>

namespace neural_network
{

class NeuralNetwork final
    : public INeuralNetwork
    , private boost::noncopyable
{
public:
    explicit NeuralNetwork(IDetectionConsumer& detection_consumer);

private:
    void OnImage(cv::Mat&& image) override;

    IDetectionConsumer& m_detection_consumer;

    std::shared_ptr<yolo::IYolo> m_yolo;

    std::atomic<bool>       m_is_running;
    std::mutex              m_image_queue_mutex;
    std::condition_variable m_image_queue_cv;

    std::queue<cv::Mat> m_image_queue;

    core::Thread m_thread;
};

} // namespace neural_network
