#pragma once

#include "matrix.hpp"

class ErrorFunction {
public:
  virtual TFloat apply(const Matrix &got, const Matrix &expected) = 0;
  virtual Matrix derivative(const Matrix &got, const Matrix &expected) = 0;
};
