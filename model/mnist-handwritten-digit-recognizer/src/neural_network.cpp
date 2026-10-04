#include <iostream>
#include <random>

#include "neural_network.hpp"

using namespace std;

void randomize_matrix(Matrix &matrix, random_device &rd,
                      uniform_real_distribution<TFloat> &distribution) {
  for (size_t i = 0; i != matrix.size(); i++) {
    matrix(i) = distribution(rd);
  }
}

void NeuralNetwork::randomize_parameters(TFloat min, TFloat max) {
  random_device rd;
  uniform_real_distribution distribution(min, max);

  randomize_matrix(this->_parameters, rd, distribution);
}

NeuralNetwork::NeuralNetwork(vector<shared_ptr<Layer>> layers,
                             shared_ptr<ErrorFunction> error_function)
    : _layers(layers), _error_function(error_function) {
  size_t parameters_count = 0;

  for (size_t i = 0; i != this->_layers.size(); i++) {
    parameters_count += this->_layers[i]->parameters_count();
  }

  this->_parameters = Matrix(parameters_count, 1);
  this->_gradient = Matrix(parameters_count, 1);

  this->_gradient.zeroize();

  size_t parameters_offset = 0;

  for (size_t i = 0; i != this->_layers.size(); i++) {
    this->_layers[i]->set_parameters(this->_parameters.data() +
                                     parameters_offset);
    this->_layers[i]->set_gradient(this->_gradient.data() + parameters_offset);

    parameters_offset += this->_layers[i]->parameters_count();
  }
}

Matrix NeuralNetwork::forward(Matrix input) {
  this->_output = std::move(input);

  for (size_t i = 0; i != this->_layers.size(); i++) {
    this->_output = this->_layers[i]->forward(std::move(this->_output));
  }

  return this->_output;
}

TFloat NeuralNetwork::expect(Matrix expected_output) {
  this->_expected_output = std::move(expected_output);

  return this->_error_function->apply(this->_output, this->_expected_output);
}

void NeuralNetwork::backward() {
  Matrix layer_error =
      this->_error_function->derivative(this->_output, this->_expected_output);

  for (size_t i = this->_layers.size() - 1; i != 0; i--) {
    auto &layer = this->_layers[i];

    layer->backward(layer_error);

    layer_error = layer->previous_layer_error(layer_error);
  }

  this->_layers.front()->backward(layer_error);
}

Matrix &NeuralNetwork::parameters() { return this->_parameters; }

Matrix &NeuralNetwork::gradient() { return this->_gradient; }
