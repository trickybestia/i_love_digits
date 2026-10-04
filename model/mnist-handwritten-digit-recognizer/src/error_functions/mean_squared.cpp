#include <cmath>

#include "mean_squared.hpp"

using namespace std;

TFloat MeanSquared::apply(const Matrix &got, const Matrix &expected) {
  if (got.rows() != expected.rows() || got.cols() != expected.cols())
    throw exception();

  TFloat result = 0.0;

  for (size_t i = 0; i != got.size(); i++) {
    result += pow(got(i) - expected(i), 2.0);
  }

  result /= got.size();

  return result;
}

Matrix MeanSquared::derivative(const Matrix &got, const Matrix &expected) {
  if (got.rows() != expected.rows() || got.cols() != expected.cols())
    throw exception();

  Matrix result(got.rows(), got.cols());

  for (size_t i = 0; i != got.size(); i++) {
    result(i) = 2.0 * (got(i) - expected(i)) / got.size();
  }

  return result;
}
