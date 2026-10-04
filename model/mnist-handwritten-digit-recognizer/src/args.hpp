#pragma once

#include <filesystem>
#include <optional>

#include "tfloat.hpp"

struct Args {
  std::optional<std::filesystem::path> model, dataset, dump_dataset;
  std::optional<std::string> optimizer;
  bool train, train_existing_model, train_with_noise, compute_test_accuracy,
      showcase;
  TFloat learning_rate, random_parameter_min, random_parameter_max, beta1,
      beta2;
};

Args parse_args(int argc, char **argv);
