#pragma once

#include <iostream>
#include <vector> 

class Ant;

struct Neuron
{
    std::vector<double> weights;

    double bias;
    double output{0.0};

};

class NeuralNetwork
{
public:
    NeuralNetwork(int _nuerons, int _layers);
    ~NeuralNetwork() {}

    int neurons;
    int layers;

    void take_action(double dt,Ant &ant);
private:
    const int INPUTS = 1;
    const int OUTPUTS = 2;
    const int RANDOM_NEURON_BIAS_RANGE = 11;
    const int RANDOM_NEURON_WEIGHT_RANGE = 11;
   
    const int RANDOM_NEURON_BIAS_OFFSET = -5;
    const int RANDOM_NEURON_WEIGHT_OFFSET = -5;
    std::vector<std::vector<Neuron>> network;
    void move(double output, Ant &ant);
    void calculate_neuron_output(const std::vector<double>& inputs, Neuron &neuron);

    void rotate_ant(Ant &ant, double value, double dt);

};
