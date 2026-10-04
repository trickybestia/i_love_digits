#pragma once

#include "../error_function.hpp"
#include "../layers/activation_function.hpp"

class CrossEntropySoftmax : public ErrorFunction, public ActivationFunction {
private:
  Matrix _expected_output, _output;

public:
  virtual TFloat apply(const Matrix &got, const Matrix &expected) override;
  virtual Matrix derivative(const Matrix &got, const Matrix &expected) override;

  virtual Matrix forward(Matrix input) override;

  virtual Matrix previous_layer_error(const Matrix &layer_error) override;
};
