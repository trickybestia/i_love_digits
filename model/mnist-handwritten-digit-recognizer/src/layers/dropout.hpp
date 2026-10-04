#pragma once

#include <random>

#include "../layer.hpp"

class Dropout : public Layer {
private:
  Matrix _factors;
  std::mt19937_64 _rng;
  std::bernoulli_distribution _distribution;
  TFloat _value;

public:
  Dropout(float probability, TFloat value = 0.0);

  virtual size_t parameters_count() const override;

  virtual void set_parameters(TFloat *value) override;
  virtual void set_gradient(TFloat *value) override;

  virtual Matrix forward(Matrix input) override;

  virtual void backward(const Matrix &layer_error) override;

  virtual Matrix previous_layer_error(const Matrix &layer_error) override;
};
