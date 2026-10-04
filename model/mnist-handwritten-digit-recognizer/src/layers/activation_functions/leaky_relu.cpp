#include "leaky_relu.hpp"

LeakyReLU::LeakyReLU(TFloat factor) : _factor(factor) {}

TFloat LeakyReLU::apply(TFloat x) const {
  if (x > 0.0)
    return x;

  return this->_factor * x;
}

TFloat LeakyReLU::derivative(TFloat x) const {
  if (x > 0.0)
    return 1.0;

  return this->_factor;
}
