#pragma once

#include <cstdint>

#include "../matrix.hpp"

namespace mnist {
struct DatasetEntry {
private:
  uint8_t _label;
  Matrix _image;

public:
  DatasetEntry(uint8_t label, Matrix image) : _label(label), _image(image) {}

  uint8_t label() const { return this->_label; }
  const Matrix &image() const { return this->_image; }
};
} // namespace mnist
