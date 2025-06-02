#pragma once

#include <jpeg_decoder/IImageConsumer.hpp>

namespace neural_network
{

class INeuralNetwork : public jpeg_decoder::IImageConsumer
{
};

} // namespace neural_network
