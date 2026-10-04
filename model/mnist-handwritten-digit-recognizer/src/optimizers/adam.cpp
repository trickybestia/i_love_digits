#include <cmath>

#include "adam.hpp"

Adam::Adam(Function &function, TFloat learning_rate, TFloat beta1, TFloat beta2,
           TFloat eps)
    : Optimizer(function), _learning_rate(learning_rate), _beta1(beta1),
      _beta2(beta2), _eps(eps), _iteration(1),
      _parameters_m(function.parameters().rows(), function.parameters().cols()),
      _parameters_v(function.parameters().rows(),
                    function.parameters().cols()) {
  this->_parameters_m.zeroize();
  this->_parameters_v.zeroize();
}

void Adam::step() {
  Matrix &parameters_gradient = this->_function.gradient();

  this->_parameters_m *= this->_beta1;
  this->_parameters_m += parameters_gradient * (1.0 - this->_beta1);
  this->_parameters_v *= this->_beta2;
  this->_parameters_v += parameters_gradient.pow(2.0) * (1 - this->_beta2);

  Matrix parameters_mt =
             this->_parameters_m / (1.0 - pow(this->_beta1, this->_iteration)),
         parameters_vt =
             this->_parameters_v / (1.0 - pow(this->_beta2, this->_iteration));

  this->_function.parameters() -= parameters_mt /
                                  (parameters_vt.pow(0.5) + this->_eps) *
                                  this->_learning_rate;
}
