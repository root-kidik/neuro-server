#include <jpeg_encoder/Factory.hpp>
#include <jpeg_encoder/JpegEncoder.hpp>

namespace jpeg_encoder
{

std::unique_ptr<IJpegEncoder> MakeJpegEncoder(IJpegConsumer& jpeg_consumer)
{
    return std::make_unique<JpegEncoder>(jpeg_consumer);
}

} // namespace jpeg_encoder
