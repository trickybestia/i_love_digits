#include "simple_activation_function.hpp"

Matrix SimpleActivationFunction::forward(Matrix input) {
  Matrix result(input.rows(), input.cols());

  for (size_t i = 0; i != input.size(); i++) {
    result(i) = this->apply(input(i));
  }

  this->_input = std::move(input);

  return result;
}

Matrix
SimpleActivationFunction::previous_layer_error(const Matrix &layer_error) {
  Matrix result(layer_error.rows(), layer_error.cols());

  for (size_t i = 0; i != layer_error.size(); i++) {
    result(i) = layer_error(i) * this->derivative(this->_input(i));
  }

  return result;
}
