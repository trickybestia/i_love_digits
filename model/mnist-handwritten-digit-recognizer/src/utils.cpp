#include <fstream>

#include "exceptions.hpp"
#include "utils.hpp"

using namespace std;
using namespace std::filesystem;

void save_matrix(const Matrix &matrix, const path &path) {
  ofstream file(path);

  file.exceptions(ofstream::failbit);

  file.write(reinterpret_cast<const char *>(matrix.data()),
             matrix.size() * sizeof(TFloat));
}

void load_matrix(Matrix &matrix, const path &path) {
  ifstream file(path);

  if (!file.good()) {
    throw FileNotFoundException(path);
  }

  file.seekg(0, ifstream::end);

  auto file_size = file.tellg();

  file.seekg(0);

  if (static_cast<decltype(file_size)>(matrix.size() * sizeof(TFloat)) !=
      file_size) {
    throw FileSizeMismatchException(path, matrix.size() * sizeof(TFloat),
                                    file_size);
  }

  file.read(reinterpret_cast<char *>(matrix.data()),
            matrix.size() * sizeof(TFloat));
}

void load_matrix_float(Matrix &matrix, const path &path) {
  ifstream file(path);

  if (!file.good()) {
    throw FileNotFoundException(path);
  }

  file.seekg(0, ifstream::end);

  auto file_size = file.tellg();

  file.seekg(0);

  if (static_cast<decltype(file_size)>(matrix.size() * sizeof(float)) !=
      file_size) {
    throw FileSizeMismatchException(path, matrix.size() * sizeof(float),
                                    file_size);
  }

  float *float_data = new float[matrix.size()];

  file.read(reinterpret_cast<char *>(float_data),
            matrix.size() * sizeof(float));

  for (size_t i = 0; i != matrix.size(); i++) {
    matrix(i) = TFloat(float_data[i]);
  }

  delete[] float_data;
}

size_t max_item_index(const Matrix &matrix) {
  size_t result = 0;

  for (size_t i = 0; i != matrix.size(); i++) {
    if (matrix(i) > matrix(result)) {
      result = i;
    }
  }

  return result;
}
