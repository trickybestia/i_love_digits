#pragma once

#include "../error_function.hpp"

class MeanSquared : public ErrorFunction {
  virtual TFloat apply(const Matrix &got, const Matrix &expected) override;
  virtual Matrix derivative(const Matrix &got, const Matrix &expected) override;
};
