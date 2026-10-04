#pragma once

#include "../layer.hpp"
#include "../matrix.hpp"

class ActivationFunction : public Layer {
public:
  virtual size_t parameters_count() const override;

  virtual void set_parameters(TFloat *value) override;
  virtual void set_gradient(TFloat *value) override;

  virtual void backward(const Matrix &layer_error) override;
};
