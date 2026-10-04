#pragma once

#include <filesystem>

#include "dataset.hpp"

void dump_dataset(const mnist::Dataset &dataset,
                  const std::filesystem::path &root);
