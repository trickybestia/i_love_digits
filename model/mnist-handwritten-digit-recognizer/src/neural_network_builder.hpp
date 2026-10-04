#pragma once

#include "neural_network.hpp"

class NeuralNetworkBuilder {
private:
  std::vector<std::shared_ptr<Layer>> _layers;
  std::shared_ptr<ErrorFunction> _error_function;

public:
  NeuralNetworkBuilder(std::shared_ptr<ErrorFunction> error_function);

  void add_layer(std::shared_ptr<Layer> layer);

  NeuralNetwork build() const;
};
