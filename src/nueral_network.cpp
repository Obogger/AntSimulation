#include "nueral_network.hpp"
#include "colony.hpp"


NeuralNetwork::NeuralNetwork(int _nuerons, int _layers) : neurons(_nuerons), layers(_layers) 
{
    //Input layer
    std::vector<Neuron> layer;
    for(int j = 0; j < neurons; ++j) 
    {
        Neuron neuron;
        for(int k = 0; k < INPUTS; ++k) 
        {
            double weight = static_cast<double>(rand() % RANDOM_NEURON_WEIGHT_RANGE + RANDOM_NEURON_WEIGHT_OFFSET);

            neuron.weights.push_back(weight);
        }
        double bias = static_cast<double>(rand() % RANDOM_NEURON_BIAS_RANGE + RANDOM_NEURON_BIAS_OFFSET);
        neuron.bias = bias;
        
        layer.push_back(neuron);
    }
    network.push_back(layer);

    // Hidden layers
    for(int i = 0; i < layers - 1; ++i) 
    {
        std::vector<Neuron> layer;
        for(int j = 0; j < neurons; ++j) 
        {
            Neuron neuron;
            for(int k = 0; k < neurons; ++k) 
            {
                double weight = static_cast<double>(rand() % RANDOM_NEURON_WEIGHT_RANGE + RANDOM_NEURON_WEIGHT_OFFSET);

                neuron.weights.push_back(weight);
            }
            double bias = static_cast<double>(rand() % RANDOM_NEURON_BIAS_RANGE + RANDOM_NEURON_BIAS_OFFSET);
            neuron.bias = bias;
            
            layer.push_back(neuron);
        }
        network.push_back(layer);
    }

    // Output layer
    std::vector<Neuron> output_layer;

    for(int i = 0; i < OUTPUTS; i++)
    {
        Neuron output_neuron;
        for(int j = 0; j < neurons; j++) 
        {
            double weight = static_cast<double>(rand() % RANDOM_NEURON_WEIGHT_RANGE + RANDOM_NEURON_WEIGHT_OFFSET);

            output_neuron.weights.push_back(weight);
        }
        double bias = static_cast<double>(rand() % RANDOM_NEURON_BIAS_RANGE + RANDOM_NEURON_BIAS_OFFSET);
        output_neuron.bias = bias;

        output_layer.push_back(output_neuron);
    }
    network.push_back(output_layer);

}

void NeuralNetwork::take_action(double dt, Ant &ant)
{
    std::vector<double> inputs;

    for(int i = 0; i < INPUTS; i++)
    {
        inputs.push_back(0.0);
    }

    for(int j = 0; j < layers; j++)
    {
        for(int i = 0; i < neurons; i++)
        {
            calculate_neuron_output(inputs, network[j][i]);
        }

        inputs.clear();
        for(int i = 0; i < neurons; i++)
        {
            inputs.push_back(network[j][i].output);
        }
    }


    for(int i = 0; i < OUTPUTS; i++)
    {
        calculate_neuron_output(inputs, network.back()[i]);
        double output = network.back()[i].output;

        switch (i)
        {
        case 0:
            move(output, ant);
            break;
        case 1:
            rotate_ant(ant, output, dt);
            break;
        
        default:
            break;
        }

    }
}

void NeuralNetwork::move(double output, Ant &ant)
{

    double radians = ant.rotation * M_PI / 180.0;

    ant.veocity.x = std::cos(radians) * (output * ant.speed);
    ant.veocity.y = std::sin(radians) * (output * ant.speed);
}

void NeuralNetwork::rotate_ant(Ant &ant, double value, double dt)
{
    ant.rotation += value * ant.rotation_speed * dt;
    
}

void NeuralNetwork::calculate_neuron_output(const std::vector<double>& inputs, Neuron &neuron)
{
    double sum = neuron.bias;
    for(size_t i = 0; i < inputs.size(); ++i) 
    {
        sum += neuron.weights[i] * inputs[i];
    }
    neuron.output = std::tanh(sum / 2.0);
}