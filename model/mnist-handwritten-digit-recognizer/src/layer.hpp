#pragma once

#include "matrix.hpp"

class Layer {
public:
  virtual size_t parameters_count() const = 0;

  virtual void set_parameters(TFloat *value) = 0;
  virtual void set_gradient(TFloat *value) = 0;

  virtual Matrix forward(Matrix input) = 0;
  virtual void backward(const Matrix &layer_error) = 0;

  virtual Matrix previous_layer_error(const Matrix &layer_error) = 0;
};
