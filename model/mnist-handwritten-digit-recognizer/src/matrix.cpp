#include <random>

#include "matrix.hpp"

using namespace std;

void Matrix::swap(Matrix &other) {
  std::swap(other._rows, this->_rows);
  std::swap(other._cols, this->_cols);
  std::swap(other._size, this->_size);
  std::swap(other._owns_data, this->_owns_data);
  std::swap(other._data, this->_data);
}

Matrix::Matrix()
    : _rows(0), _cols(0), _size(0), _owns_data(false), _data(nullptr) {}

Matrix::Matrix(size_t rows, size_t cols)
    : _rows(rows), _cols(cols), _size(rows * cols), _owns_data(false),
      _data(nullptr) {
  if (this->size() != 0) {
    this->_data = new TFloat[this->size()];
    this->_owns_data = true;
  }
}

Matrix::Matrix(size_t rows, size_t cols, TFloat *data)
    : _rows(rows), _cols(cols), _size(rows * cols), _owns_data(false),
      _data(data) {}

Matrix::Matrix(const Matrix &other)
    : _rows(other._rows), _cols(other._cols), _size(other._size),
      _owns_data(other._owns_data), _data(other._data) {
  if (other._owns_data) {
    this->_data = new TFloat[other.size()];

    copy(other._data, other._data + other.size(), this->_data);
  }
}

Matrix::Matrix(Matrix &&other) : Matrix() { this->swap(other); }

Matrix::~Matrix() {
  if (this->_owns_data)
    delete[] this->_data;
}

size_t Matrix::rows() const { return this->_rows; }
size_t Matrix::cols() const { return this->_cols; }
size_t Matrix::size() const { return this->_size; }

const TFloat *Matrix::data() const { return this->_data; }
TFloat *Matrix::data() { return this->_data; }

TFloat Matrix::operator()(size_t i, size_t j) const {
  return (*this)(i * this->_cols + j);
}

TFloat &Matrix::operator()(size_t i, size_t j) {
  return (*this)(i * this->_cols + j);
}

TFloat Matrix::operator()(size_t i) const {
  if (i >= this->_size)
    throw exception();

  return this->_data[i];
}

TFloat &Matrix::operator()(size_t i) {
  if (i >= this->_size)
    throw exception();

  return this->_data[i];
}

Matrix Matrix::transpose() const {
  Matrix result(this->_cols, this->_rows);

  for (size_t i = 0; i != this->_rows; i++) {
    for (size_t j = 0; j != this->_cols; j++) {
      result(j, i) = (*this)(i, j);
    }
  }

  return result;
}

Matrix Matrix::dot(const Matrix &other) const {
  if (this->_cols != other._rows)
    throw exception();

  Matrix result(this->_rows, other._cols);

  result.zeroize();

#pragma omp parallel for
  for (size_t i = 0; i != this->_rows; i++) {
    for (size_t k = 0; k != this->_cols; k++) {
      TFloat this_ik = (*this)(i, k);

      for (size_t j = 0; j != other._cols; j++) {
        result(i, j) += this_ik * other(k, j);
      }
    }
  }

  return result;
}

Matrix Matrix::pow(TFloat power) {
  Matrix result(this->_rows, this->_cols);

#pragma omp parallel for
  for (size_t i = 0; i != this->_size; i++) {
    result._data[i] = ::pow(this->_data[i], power);
  }

  return result;
}

void Matrix::zeroize() {
  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] = 0.0;
  }
}

void Matrix::operator*=(TFloat factor) {
  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] *= factor;
  }
}

void Matrix::operator/=(TFloat factor) {
  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] /= factor;
  }
}

void Matrix::operator+=(TFloat number) {
  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] += number;
  }
}

void Matrix::operator-=(TFloat number) {
  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] -= number;
  }
}

void Matrix::operator+=(const Matrix &other) {
  if (this->_cols != other._cols || this->_rows != other._rows)
    throw exception();

  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] += other._data[i];
  }
}

void Matrix::operator-=(const Matrix &other) {
  if (this->_cols != other._cols || this->_rows != other._rows)
    throw exception();

  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] -= other._data[i];
  }
}

void Matrix::operator*=(const Matrix &other) {
  if (this->_cols != other._cols || this->_rows != other._rows)
    throw exception();

  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] *= other._data[i];
  }
}

void Matrix::operator/=(const Matrix &other) {
  if (this->_cols != other._cols || this->_rows != other._rows)
    throw exception();

  for (size_t i = 0; i != this->_size; i++) {
    this->_data[i] /= other._data[i];
  }
}

Matrix Matrix::operator*(TFloat factor) const {
  Matrix result = *this;

  result *= factor;

  return result;
}

Matrix Matrix::operator/(TFloat factor) const {
  Matrix result = *this;

  result /= factor;

  return result;
}

Matrix Matrix::operator+(TFloat number) const {
  Matrix result = *this;

  result += number;

  return result;
}

Matrix Matrix::operator-(TFloat number) const {
  Matrix result = *this;

  result -= number;

  return result;
}

Matrix Matrix::operator+(const Matrix &other) const {
  Matrix result = *this;

  result += other;

  return result;
}

Matrix Matrix::operator-(const Matrix &other) const {
  Matrix result = *this;

  result -= other;

  return result;
}

Matrix Matrix::operator*(const Matrix &other) const {
  Matrix result = *this;

  result *= other;

  return result;
}

Matrix Matrix::operator/(const Matrix &other) const {
  Matrix result = *this;

  result /= other;

  return result;
}

Matrix &Matrix::operator=(const Matrix &other) {
  Matrix copied_other(other);

  this->swap(copied_other);

  return *this;
}

Matrix &Matrix::operator=(Matrix &&other) {
  Matrix empty;

  this->swap(empty);
  this->swap(other);

  return *this;
}
