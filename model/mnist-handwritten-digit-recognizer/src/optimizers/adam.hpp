#pragma once

#include "../optimizer.hpp"

class Adam : public Optimizer {
private:
  TFloat _learning_rate, _beta1, _beta2, _eps;
  size_t _iteration;

  Matrix _parameters_m, _parameters_v;

public:
  Adam(Function &function, TFloat learning_rate, TFloat beta1 = 0.9,
       TFloat beta2 = 0.999, TFloat eps = 1e-5);

  virtual void step() override;
};
