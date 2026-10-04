#include "sgd.hpp"

SGD::SGD(Function &function, TFloat learning_rate)
    : Optimizer(function), _learning_rate(learning_rate) {}

void SGD::step() {
  Matrix &parameters_gradient = this->_function.gradient();

  this->_function.parameters() -= parameters_gradient * this->_learning_rate;
}
