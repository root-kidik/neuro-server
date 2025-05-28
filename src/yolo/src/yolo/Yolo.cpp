#include <thread>

#include <yolo/Yolo.hpp>

namespace Ort
{
template <>
struct TypeToTensorType<half>
{
    static constexpr ONNXTensorElementDataType type = ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16;
};
} // namespace Ort

namespace yolo
{

Yolo::Yolo(const YoloInitParams& params)
    : m_env{ORT_LOGGING_LEVEL_WARNING, "Yolo"}
    , m_classes_size{2}
    , m_image_size{640, 640}
    , m_float_blob(m_image_size.x * m_image_size.y * 3)
    , m_half_blob(m_image_size.x * m_image_size.y * 3)
    , m_input_node_dims{1, 3, m_image_size.x, m_image_size.y}
    , m_padding_yolo_color{114, 114, 114}
{
    m_rectangle_confidence_threshold = params.rectangle_confidence_threshold;
    m_non_max_suppression_threshold  = params.non_max_suppression_threshold;
    m_is_cuda_enabled                = params.is_cuda_enabled;
    m_is_half_enabled                = params.is_half_enabled;

    Ort::SessionOptions sessionm_options;

    if (params.is_cuda_enabled)
    {
        OrtCUDAProviderOptions cudam_options;
        cudam_options.device_id = 0;
        sessionm_options.AppendExecutionProvider_CUDA(cudam_options);
    }

    sessionm_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    sessionm_options.SetIntraOpNumThreads(std::thread::hardware_concurrency());
    sessionm_options.SetLogSeverityLevel(3);

    m_session = std::make_unique<Ort::Session>(m_env, params.model_path.c_str(), sessionm_options);

    Ort::AllocatorWithDefaultOptions allocator;

    for (std::size_t i = 0; i < m_session->GetInputCount(); i++)
    {
        auto  input_node_name = m_session->GetInputNameAllocated(i, allocator);
        char* temp_buf        = new char[50];
        std::strcpy(temp_buf, input_node_name.get());
        m_input_node_names.push_back(temp_buf);
    }

    for (std::size_t i = 0; i < m_session->GetOutputCount(); i++)
    {
        Ort::AllocatedStringPtr output_node_name = m_session->GetOutputNameAllocated(i, allocator);
        char*                   temp_buf         = new char[50];
        std::strcpy(temp_buf, output_node_name.get());
        m_output_node_names.push_back(temp_buf);
    }
}

Yolo::~Yolo()
{
    for (auto input_node_name : m_input_node_names)
        delete[] input_node_name;

    for (auto output_node_name : m_output_node_names)
        delete[] output_node_name;
}

std::vector<YoloPrediction> Yolo::Run(const cv::Mat& image)
{
    auto preprocessed_image = PreProcess(image);

    if (m_is_cuda_enabled && m_is_half_enabled)
    {
        WriteImageToBlob(preprocessed_image, m_half_blob);
        return TensorProcess(m_half_blob);
    }
    else
    {
        WriteImageToBlob(preprocessed_image, m_float_blob);
        return TensorProcess(m_float_blob);
    }
}

cv::Mat Yolo::PreProcess(const cv::Mat& image)
{
    cv::Mat resized;

    if (image.cols >= image.rows)
    {
        m_resize_scales = image.cols / static_cast<float>(m_image_size.x);
        cv::resize(image, resized, cv::Size{m_image_size.x, static_cast<int>(image.rows / m_resize_scales)});
    }
    else
    {
        m_resize_scales = image.rows / static_cast<float>(m_image_size.y);
        cv::resize(image, resized, cv::Size{static_cast<int>(image.cols / m_resize_scales), m_image_size.y});
    }

    const int pad_w = m_image_size.x - resized.cols;
    const int pad_h = m_image_size.y - resized.rows;

    cv::copyMakeBorder(resized, resized, 0, pad_h, 0, pad_w, cv::BORDER_CONSTANT, m_padding_yolo_color);

    return resized;
}

template <typename T>
void Yolo::WriteImageToBlob(const cv::Mat& image, std::vector<T>& blob)
{
    for (int c = 0; c < 3; c++)
        for (int h = 0; h < m_image_size.x; h++)
            for (int w = 0; w < m_image_size.y; w++)
                blob[c * m_image_size.x * m_image_size.y + h * m_image_size.x + w] = (image.at<cv::Vec3b>(h, w)[c]) / 255.0f;
}

template <typename T>
std::vector<YoloPrediction> Yolo::TensorProcess(std::vector<T>& blob)
{
    std::vector<YoloPrediction> predictions;

    auto input_tensor = Ort::Value::CreateTensor<T>(Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU),
                                                    blob.data(),
                                                    3 * m_image_size.x * m_image_size.y,
                                                    m_input_node_dims.data(),
                                                    m_input_node_dims.size());

    auto output_tensor = m_session->Run(m_options,
                                        m_input_node_names.data(),
                                        &input_tensor,
                                        1,
                                        m_output_node_names.data(),
                                        m_output_node_names.size());

    auto type_info        = output_tensor.front().GetTypeInfo();
    auto tensor_info      = type_info.GetTensorTypeAndShapeInfo();
    auto output_node_dims = tensor_info.GetShape();
    auto output           = output_tensor.front().template GetTensorMutableData<T>();

    auto signal_result_num = output_node_dims[1]; // 7
    auto stride_num        = output_node_dims[2]; // 8400

    std::vector<int>      class_ids;
    std::vector<float>    confidences;
    std::vector<cv::Rect> boxes;

    cv::Mat raw_data;

    if (m_is_cuda_enabled && m_is_half_enabled)
    {
        raw_data = cv::Mat{static_cast<int>(signal_result_num), static_cast<int>(stride_num), CV_16F, output};
        raw_data.convertTo(raw_data, CV_32F);
    }
    else
    {
        raw_data = cv::Mat{static_cast<int>(signal_result_num), static_cast<int>(stride_num), CV_32F, output};
    }

    // Note:
    // ultralytics add transpose operator to the output of yolov8 model.which make yolov8/v5/v7 has same shape
    // https://github.com/ultralytics/assets/releases/download/v8.3.0/yolov8n.pt
    raw_data = raw_data.t();

    auto* data = reinterpret_cast<float*>(raw_data.data);

    for (int i = 0; i < stride_num; ++i)
    {
        auto* classes_scores = data + 4;

        cv::Mat   scores{1, m_classes_size, CV_32FC1, classes_scores};
        cv::Point class_id;
        double    max_class_score;

        cv::minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

        if (max_class_score > m_rectangle_confidence_threshold)
        {
            confidences.push_back(max_class_score);
            class_ids.push_back(class_id.x);

            auto x = data[0];
            auto y = data[1];
            auto w = data[2];
            auto h = data[3];

            auto left = static_cast<int>((x - 0.5 * w) * m_resize_scales);
            auto top  = static_cast<int>((y - 0.5 * h) * m_resize_scales);

            auto width  = static_cast<int>(w * m_resize_scales);
            auto height = static_cast<int>(h * m_resize_scales);

            boxes.emplace_back(left, top, width, height);
        }

        data += signal_result_num;
    }

    std::vector<int> non_max_suppressions;
    cv::dnn::NMSBoxes(boxes, confidences, m_rectangle_confidence_threshold, m_non_max_suppression_threshold, non_max_suppressions);

    for (auto idx : non_max_suppressions)
        predictions.emplace_back(confidences[idx], static_cast<TargetType>(class_ids[idx]), boxes[idx]);

    return predictions;
}

} // namespace yolo
