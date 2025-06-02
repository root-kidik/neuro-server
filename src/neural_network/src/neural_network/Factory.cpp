#include <neural_network/Factory.hpp>
#include <neural_network/NeuralNetwork.hpp>

namespace neural_network
{

std::unique_ptr<INeuralNetwork> MakeNeuralNetwork(IDetectionConsumer& detection_consumer)
{
    return std::make_unique<NeuralNetwork>(detection_consumer);
}

} // namespace neural_network
