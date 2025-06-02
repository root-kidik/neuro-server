#pragma once

#include <memory>

#include <jpeg_encoder/IJpegConsumer.hpp>
#include <jpeg_encoder/IJpegEncoder.hpp>

namespace jpeg_encoder
{

std::unique_ptr<IJpegEncoder> MakeJpegEncoder(IJpegConsumer& jpeg_consumer);

}
