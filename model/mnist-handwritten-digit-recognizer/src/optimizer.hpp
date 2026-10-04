#pragma once

#include "function.hpp"

class Optimizer {
protected:
  Function &_function;

  Optimizer(Function &function) : _function(function) {}

public:
  virtual void step() = 0;
};
