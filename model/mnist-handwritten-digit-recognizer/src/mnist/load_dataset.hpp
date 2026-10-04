#pragma once

#include <filesystem>

#include "dataset.hpp"

namespace mnist {
Dataset load_dataset(std::filesystem::path path);
} // namespace mnist
