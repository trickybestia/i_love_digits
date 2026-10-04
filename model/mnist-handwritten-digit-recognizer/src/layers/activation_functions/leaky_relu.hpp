#pragma once

#include "../simple_activation_function.hpp"

class LeakyReLU : public SimpleActivationFunction {
private:
  const TFloat _factor;

public:
  LeakyReLU(TFloat factor);

  virtual TFloat apply(TFloat x) const override;
  virtual TFloat derivative(TFloat x) const override;
};
