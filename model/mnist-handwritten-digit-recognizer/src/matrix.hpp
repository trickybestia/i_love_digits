#pragma once

#include <cstddef>
#include <vector>

#include "tfloat.hpp"

class Matrix {
private:
  size_t _rows, _cols, _size;
  bool _owns_data;
  TFloat *_data;

  void swap(Matrix &other);

public:
  Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(size_t rows, size_t cols, TFloat *data);

  Matrix(const Matrix &other);
  Matrix(Matrix &&other);

  ~Matrix();

  size_t rows() const;
  size_t cols() const;
  size_t size() const;

  const TFloat *data() const;
  TFloat *data();

  TFloat operator()(size_t i, size_t j) const;
  TFloat &operator()(size_t i, size_t j);

  TFloat operator()(size_t i) const;
  TFloat &operator()(size_t i);

  Matrix transpose() const;
  Matrix dot(const Matrix &other) const;
  Matrix pow(TFloat power);

  void zeroize();

  void operator*=(TFloat factor);
  void operator/=(TFloat factor);
  void operator+=(TFloat number);
  void operator-=(TFloat number);

  void operator*=(const Matrix &other);
  void operator/=(const Matrix &other);
  void operator+=(const Matrix &other);
  void operator-=(const Matrix &other);

  Matrix operator*(TFloat factor) const;
  Matrix operator/(TFloat factor) const;
  Matrix operator+(TFloat number) const;
  Matrix operator-(TFloat number) const;

  Matrix operator*(const Matrix &other) const;
  Matrix operator/(const Matrix &other) const;
  Matrix operator+(const Matrix &other) const;
  Matrix operator-(const Matrix &other) const;

  Matrix &operator=(const Matrix &other);
  Matrix &operator=(Matrix &&other);
};
