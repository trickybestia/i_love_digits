#pragma once

#include <exception>
#include <filesystem>
#include <optional>
#include <string>

class FileException : public std::exception {
private:
  std::filesystem::path _path;

protected:
  std::string _message;

public:
  FileException(std::filesystem::path path, bool initialize_message = true);

  const std::filesystem::path &path() const;

  virtual const char *what() const noexcept override;
};

class FileNotFoundException : public FileException {
public:
  FileNotFoundException(std::filesystem::path path,
                        bool initialize_message = true);
};

class FileSizeMismatchException : public FileException {
private:
  size_t _expected_size, _actual_size;

public:
  FileSizeMismatchException(std::filesystem::path path, size_t expected_size,
                            size_t actual_size, bool initialize_message = true);

  size_t expected_size() const;
  size_t actual_size() const;
};
