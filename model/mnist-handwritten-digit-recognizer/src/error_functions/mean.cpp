#include <cmath>

#include "mean.hpp"

using namespace std;

TFloat Mean::apply(const Matrix &got, const Matrix &expected) {
  if (got.rows() != expected.rows() || got.cols() != expected.cols())
    throw exception();

  TFloat result = 0.0;

  for (size_t i = 0; i != got.size(); i++) {
    result += abs(got(i) - expected(i));
  }

  result /= got.size();

  return result;
}

Matrix Mean::derivative(const Matrix &got, const Matrix &expected) {
  if (got.rows() != expected.rows() || got.cols() != expected.cols())
    throw exception();

  Matrix result(got.rows(), got.cols());

  for (size_t i = 0; i != got.size(); i++) {
    if (got(i) >= expected(i)) {
      result(i) = 1.0;
    } else {
      result(i) = -1.0;
    }
  }

  result /= got.size();

  return result;
}
