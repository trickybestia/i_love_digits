#include "linear.hpp"

Linear::Linear(size_t inputs, size_t outputs)
    : _inputs(inputs), _outputs(outputs) {}

size_t Linear::parameters_count() const {
  return (this->_inputs + 1) * this->_outputs;
}

void Linear::set_parameters(TFloat *value) {
  this->_weights = Matrix(this->_outputs, this->_inputs, value);
  this->_bias =
      Matrix(this->_outputs, 1, value + this->_inputs * this->_outputs);
}

void Linear::set_gradient(TFloat *value) {
  this->_weights_gradient = Matrix(this->_outputs, this->_inputs, value);
  this->_bias_gradient =
      Matrix(this->_outputs, 1, value + this->_inputs * this->_outputs);
}

Matrix Linear::forward(Matrix input) {
  this->_input = std::move(input);

  return this->_weights.dot(this->_input) + this->_bias;
}

void Linear::backward(const Matrix &layer_error) {
  this->_weights_gradient += layer_error.dot(this->_input.transpose());
  this->_bias_gradient += layer_error;
}

Matrix Linear::previous_layer_error(const Matrix &layer_error) {
  return this->_weights.transpose().dot(layer_error);
}
