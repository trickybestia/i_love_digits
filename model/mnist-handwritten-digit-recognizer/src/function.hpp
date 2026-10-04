#pragma once

#include "matrix.hpp"

class Function {
public:
  virtual Matrix &parameters() = 0;
  virtual Matrix &gradient() = 0;
};
