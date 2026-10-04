#include <cmath>

#include "cross_entropy_softmax.hpp"

using namespace std;

TFloat CrossEntropySoftmax::apply(const Matrix &got, const Matrix &expected) {
  if (got.rows() != expected.rows() || got.cols() != expected.cols())
    throw exception();

  TFloat result = 0.0;

  for (size_t i = 0; i != got.size(); i++) {
    result += expected(i) * log(got(i));
  }

  return -result;
}

Matrix CrossEntropySoftmax::derivative(const Matrix &, const Matrix &expected) {
  this->_expected_output = expected;

  return Matrix();
}

Matrix CrossEntropySoftmax::forward(Matrix input) {
  TFloat max_input_item = input(0);

  for (size_t i = 1; i != input.size(); i++) {
    max_input_item = max(max_input_item, input(i));
  }

  input -= max_input_item;

  Matrix exp_input(input.rows(), input.cols());
  TFloat exp_input_sum = 0.0;

  for (size_t i = 0; i != input.size(); i++) {
    exp_input(i) = exp(input(i));
    exp_input_sum += exp_input(i);
  }

  exp_input /= exp_input_sum;

  this->_output = std::move(exp_input);

  return this->_output;
}

Matrix CrossEntropySoftmax::previous_layer_error(const Matrix &) {
  return this->_output - this->_expected_output;
}
