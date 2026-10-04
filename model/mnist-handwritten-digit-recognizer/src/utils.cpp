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

size_t max_item_index(const Matrix &matrix) {
  size_t result = 0;

  for (size_t i = 0; i != matrix.size(); i++) {
    if (matrix(i) > matrix(result)) {
      result = i;
    }
  }

  return result;
}
