#pragma once

#include <cstdio>
#include <cuda_fp16.h>
#include <memory>
#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>

#include <opencv4/opencv2/opencv.hpp>
#include <yolo/IYolo.hpp>
#include <yolo/YoloInitParams.hpp>

namespace yolo
{

class Yolo final : public IYolo
{
public:
    explicit Yolo(const YoloInitParams& params);
    ~Yolo() override;

    std::vector<YoloPrediction> Run(const cv::Mat& image) override;

private:
    cv::Mat PreProcess(const cv::Mat& image);

    template <typename T>
    void WriteImageToBlob(const cv::Mat& image, std::vector<T>& blob);

    template <typename T>
    std::vector<YoloPrediction> TensorProcess(std::vector<T>& blob);

    const Ort::Env m_env;

    std::unique_ptr<Ort::Session> m_session;
    Ort::RunOptions               m_options;

    std::vector<const char*> m_input_node_names;
    std::vector<const char*> m_output_node_names;

    float m_rectangle_confidence_threshold;
    float m_non_max_suppression_threshold;
    float m_resize_scales;
    bool  m_is_cuda_enabled;
    bool  m_is_half_enabled;

    const std::uint8_t                m_classes_size;
    const cv::Point2i                 m_image_size;
    const std::array<std::int64_t, 4> m_input_node_dims;
    const cv::Scalar                  m_padding_yolo_color;

    std::vector<float> m_float_blob;
    std::vector<half>  m_half_blob;
};

} // namespace yolo
