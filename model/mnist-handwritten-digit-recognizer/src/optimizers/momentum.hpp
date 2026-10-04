#pragma once

#include "../optimizer.hpp"

class Momentum : public Optimizer {
private:
  TFloat _learning_rate, _beta;

  Matrix _parameters_v;

public:
  Momentum(Function &function, TFloat learning_rate, TFloat beta = 0.9);

  virtual void step() override;
};
