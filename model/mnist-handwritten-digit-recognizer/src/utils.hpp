#pragma once

#include <filesystem>
#include <vector>

#include "matrix.hpp"

void save_matrix(const Matrix &matrix, const std::filesystem::path &path);
void load_matrix(Matrix &matrix, const std::filesystem::path &path);

size_t max_item_index(const Matrix &matrix);
