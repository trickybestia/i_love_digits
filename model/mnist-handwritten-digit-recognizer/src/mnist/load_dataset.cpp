#include <algorithm>
#include <fstream>

#include "../exceptions.hpp"
#include "load_dataset.hpp"

using namespace std;
using namespace std::filesystem;

const string TRAIN_IMAGES_FILE_NAME = "train-images-idx3-ubyte";
const string TRAIN_LABELS_FILE_NAME = "train-labels-idx1-ubyte";

const string TEST_IMAGES_FILE_NAME = "t10k-images-idx3-ubyte";
const string TEST_LABELS_FILE_NAME = "t10k-labels-idx1-ubyte";

template <class T> void swap_endianness(T *value) {
  unsigned char *value_mem = reinterpret_cast<unsigned char *>(value);

  std::reverse(value_mem, value_mem + sizeof(T));
}

uint32_t read_big_endian(ifstream &stream) {
  uint32_t result;

  stream.read(reinterpret_cast<char *>(&result), 4);

  swap_endianness(&result);

  return result;
}

pair<vector<TFloat>, vector<mnist::DatasetEntry>>
load_dataset(path images_path, path labels_path) {
  ifstream images_file(images_path, ifstream::binary);
  ifstream labels_file(labels_path, ifstream::binary);

  images_file.exceptions(ifstream::failbit);
  labels_file.exceptions(ifstream::failbit);

  if (!images_file.good()) {
    throw FileNotFoundException(images_path);
  }
  if (!labels_file.good()) {
    throw FileNotFoundException(labels_path);
  }

  if (read_big_endian(images_file) != 2051 ||
      read_big_endian(labels_file) != 2049)
    throw exception();

  uint32_t images_count = read_big_endian(images_file);

  if (read_big_endian(labels_file) != images_count)
    throw exception();

  uint32_t rows = read_big_endian(images_file),
           cols = read_big_endian(images_file);

  vector<TFloat> matrix_data(rows * cols * images_count);
  vector<mnist::DatasetEntry> entries;

  entries.reserve(images_count);

  vector<uint8_t> image_buffer(rows * cols);
  uint8_t label;

  for (size_t i = 0; i != images_count; i++) {
    labels_file.read(reinterpret_cast<char *>(&label), 1);
    images_file.read(reinterpret_cast<char *>(image_buffer.data()),
                     image_buffer.size());

    for (size_t j = 0; j != rows * cols; j++) {
      matrix_data[i * rows * cols + j] = image_buffer[j] / 255.0;
    }

    entries.push_back(mnist::DatasetEntry(
        label, Matrix(rows * cols, 1, matrix_data.data() + i * rows * cols)));
  }

  return {std::move(matrix_data), std::move(entries)};
}

mnist::Dataset mnist::load_dataset(path path) {
  auto [train_matrix_data, train_entries] = ::load_dataset(
      path / TRAIN_IMAGES_FILE_NAME, path / TRAIN_LABELS_FILE_NAME);

  auto [test_matrix_data, test_entries] = ::load_dataset(
      path / TEST_IMAGES_FILE_NAME, path / TEST_LABELS_FILE_NAME);

  return mnist::Dataset(std::move(train_entries), std::move(test_entries),
                        std::move(train_matrix_data),
                        std::move(test_matrix_data));
}
