#pragma once

#include <random>

#include "../layer.hpp"

class UniformNoise : public Layer {
private:
  Matrix _factors;
  std::mt19937_64 _rng;
  std::bernoulli_distribution _probability_distribution;
  std::uniform_real_distribution<TFloat> _value_distribution;

public:
  UniformNoise(float probability, TFloat min, TFloat max);

  virtual size_t parameters_count() const override;

  virtual void set_parameters(TFloat *value) override;
  virtual void set_gradient(TFloat *value) override;

  virtual Matrix forward(Matrix input) override;

  virtual void backward(const Matrix &layer_error) override;

  virtual Matrix previous_layer_error(const Matrix &layer_error) override;
};
