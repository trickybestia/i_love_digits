#pragma once

#include "../optimizer.hpp"

class SGD : public Optimizer {
private:
  TFloat _learning_rate;

public:
  SGD(Function &function, TFloat learning_rate);

  virtual void step() override;
};
