#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

#include <jpeg_encoder/IJpegConsumer.hpp>
#include <jpeg_encoder/IJpegEncoder.hpp>
#include <opencv4/opencv2/opencv.hpp>

#include <boost/noncopyable.hpp>

#include <core/Thread.hpp>

namespace jpeg_encoder
{

class JpegEncoder final
    : public IJpegEncoder
    , private boost::noncopyable
{
public:
    explicit JpegEncoder(IJpegConsumer& jpeg_consumer);

private:
    void OnDetection(cv::Mat&& image, std::vector<yolo::YoloPrediction>&& predictions) override;

    IJpegConsumer& m_jpeg_consumer;

    std::atomic<bool>       m_is_running;
    std::mutex              m_image_queue_mutex;
    std::condition_variable m_image_queue_cv;

    std::queue<std::pair<cv::Mat, std::vector<yolo::YoloPrediction>>> m_image_queue;

    core::Thread m_thread;
};

} // namespace jpeg_encoder
