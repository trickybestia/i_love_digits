#include "dropout.hpp"

using namespace std;

Dropout::Dropout(float probability, TFloat value)
    : _distribution(probability), _value(value) {
  random_device rd;

  this->_rng.seed(rd());
}

size_t Dropout::parameters_count() const { return 0; }

void Dropout::set_parameters(TFloat *) {}

void Dropout::set_gradient(TFloat *) {}

void Dropout::backward(const Matrix &) {}

Matrix Dropout::forward(Matrix input) {
  Matrix result(input.rows(), input.cols());

  this->_factors = Matrix(input.rows(), input.cols());

  for (size_t i = 0; i != input.size(); i++) {
    if (this->_distribution(this->_rng)) {
      result(i) = this->_value;

      this->_factors(i) = 0.0;
    } else {
      result(i) = input(i);

      this->_factors(i) = 1.0;
    }
  }

  return result;
}

Matrix Dropout::previous_layer_error(const Matrix &layer_error) {
  return layer_error * this->_factors;
}
