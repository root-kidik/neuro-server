#pragma once

#include <cstdint>
#include <vector>

namespace jpeg_encoder
{

class IJpegConsumer
{
public:
    virtual ~IJpegConsumer() = default;

    virtual void OnJpeg(std::vector<std::uint8_t>&& jpeg) = 0;
};

} // namespace jpeg_encoder
