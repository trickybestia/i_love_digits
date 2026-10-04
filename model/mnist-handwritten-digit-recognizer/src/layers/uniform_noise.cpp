#include "uniform_noise.hpp"

using namespace std;

UniformNoise::UniformNoise(float probability, TFloat min, TFloat max)
    : _probability_distribution(probability), _value_distribution(min, max) {
  random_device rd;

  this->_rng.seed(rd());
}

size_t UniformNoise::parameters_count() const { return 0; }

void UniformNoise::set_parameters(TFloat *) {}

void UniformNoise::set_gradient(TFloat *) {}

void UniformNoise::backward(const Matrix &) {}

Matrix UniformNoise::forward(Matrix input) {
  Matrix result(input.rows(), input.cols());

  this->_factors = Matrix(input.rows(), input.cols());

  for (size_t i = 0; i != input.size(); i++) {
    if (this->_probability_distribution(this->_rng)) {
      result(i) = this->_value_distribution(this->_rng);

      this->_factors(i) = 0.0;
    } else {
      result(i) = input(i);

      this->_factors(i) = 1.0;
    }
  }

  return result;
}

Matrix UniformNoise::previous_layer_error(const Matrix &layer_error) {
  return layer_error * this->_factors;
}
