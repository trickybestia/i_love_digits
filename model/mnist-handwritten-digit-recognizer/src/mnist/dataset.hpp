#pragma once

#include <vector>

#include "dataset_entry.hpp"

namespace mnist {
class Dataset {
private:
  std::vector<TFloat> _train_matrix_data, _test_matrix_data;
  std::vector<DatasetEntry> _train_entries, _test_entries;

public:
  Dataset(std::vector<DatasetEntry> train_entries,
          std::vector<DatasetEntry> test_entries,
          std::vector<TFloat> train_matrix_data,
          std::vector<TFloat> test_matrix_data)
      : _train_matrix_data(std::move(train_matrix_data)),
        _test_matrix_data(std::move(test_matrix_data)),
        _train_entries(std::move(train_entries)),
        _test_entries(std::move(test_entries)) {}

  const std::vector<DatasetEntry> &train_entries() const {
    return this->_train_entries;
  }
  const std::vector<DatasetEntry> &test_entries() const {
    return this->_test_entries;
  }
};
} // namespace mnist
