#pragma once

#include <memory>

#include <neural_network/IDetectionConsumer.hpp>
#include <neural_network/INeuralNetwork.hpp>

namespace neural_network
{

std::unique_ptr<INeuralNetwork> MakeNeuralNetwork(IDetectionConsumer& detection_consumer);

}
