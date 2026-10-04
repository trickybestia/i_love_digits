#pragma once

#include "../layer.hpp"
#include "../matrix.hpp"
#include "activation_function.hpp"

class SimpleActivationFunction : public ActivationFunction {
private:
  Matrix _input;

public:
  virtual TFloat apply(TFloat x) const = 0;
  virtual TFloat derivative(TFloat x) const = 0;

  virtual Matrix forward(Matrix input) override;

  virtual Matrix previous_layer_error(const Matrix &layer_error) override;
};
