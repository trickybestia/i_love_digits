#pragma once

#include "../activation_function.hpp"

class Sigmoid : public ActivationFunction {
private:
  Matrix _output;

public:
  virtual Matrix forward(Matrix input) override;

  virtual Matrix previous_layer_error(const Matrix &layer_error) override;
};
