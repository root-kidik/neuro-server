#include <jpeg_decoder/Factory.hpp>
#include <jpeg_decoder/JpegDecoder.hpp>

namespace jpeg_decoder
{

std::unique_ptr<IJpegDecoder> MakeJpegDecoder(IImageConsumer& image_consumer)
{
    return std::make_unique<JpegDecoder>(image_consumer);
}

} // namespace jpeg_decoder
