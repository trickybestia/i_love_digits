#pragma once

#include "../layer.hpp"
#include "../matrix.hpp"

class Linear : public Layer {
private:
  size_t _inputs, _outputs;

  Matrix _weights, _bias, _weights_gradient, _bias_gradient, _input;

public:
  Linear(size_t inputs, size_t outputs);

  virtual size_t parameters_count() const override;

  virtual void set_parameters(TFloat *value) override;
  virtual void set_gradient(TFloat *value) override;

  virtual Matrix forward(Matrix input) override;
  virtual void backward(const Matrix &layer_error) override;

  virtual Matrix previous_layer_error(const Matrix &layer_error) override;
};
