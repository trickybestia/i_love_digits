#include <format>

#include "exceptions.hpp"

using namespace std;

namespace fs = std::filesystem;

FileException::FileException(fs::path path, bool initialize_message)
    : _path(path) {
  if (initialize_message) {
    this->_message = format("`{}`: file error", this->_path.c_str());
  }
}

const fs::path &FileException::path() const { return this->_path; }

const char *FileException::what() const noexcept {
  return this->_message.c_str();
}

FileNotFoundException::FileNotFoundException(fs::path path,
                                             bool initialize_message)
    : FileException(path, false) {
  if (initialize_message) {
    this->_message = format("`{}`: file not found", this->path().c_str());
  }
}

FileSizeMismatchException::FileSizeMismatchException(fs::path path,
                                                     size_t expected_size,
                                                     size_t actual_size,
                                                     bool initialize_message)
    : FileException(path, false), _expected_size(expected_size),
      _actual_size(actual_size) {
  if (initialize_message) {
    this->_message =
        format("`{}`: expected file with {} bytes; {} bytes got",
               this->path().c_str(), this->_expected_size, this->_actual_size);
  }
}

size_t FileSizeMismatchException::expected_size() const {
  return this->_expected_size;
}

size_t FileSizeMismatchException::actual_size() const {
  return this->_actual_size;
}
