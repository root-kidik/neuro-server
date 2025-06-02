#pragma once

#include <memory>

#include <jpeg_decoder/IImageConsumer.hpp>
#include <jpeg_decoder/IJpegDecoder.hpp>

namespace jpeg_decoder
{

std::unique_ptr<IJpegDecoder> MakeJpegDecoder(IImageConsumer& image_consumer);

}
