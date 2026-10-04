#include <cmath>

#include "sigmoid.hpp"

Matrix Sigmoid::forward(Matrix input) {
  this->_output = Matrix(input.rows(), input.cols());

  for (size_t i = 0; i != input.size(); i++) {
    this->_output(i) = 1.0 / (1.0 + exp(-input(i)));
  }

  return this->_output;
}

Matrix Sigmoid::previous_layer_error(const Matrix &layer_error) {
  Matrix result(layer_error.rows(), layer_error.cols());

  for (size_t i = 0; i != layer_error.size(); i++) {
    result(i) = layer_error(i) * this->_output(i) * (1.0 - this->_output(i));
  }

  return result;
}
