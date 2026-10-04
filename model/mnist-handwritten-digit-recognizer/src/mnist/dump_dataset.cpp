#include "dump_dataset.hpp"
#include "../utils.hpp"

using namespace std;
using namespace std::filesystem;

string create_filename(size_t index, uint8_t label) {
  return to_string(index) + "_" + to_string(static_cast<int>(label)) + ".bin";
}

void dump_dataset(const mnist::Dataset &dataset, const path &root) {
  create_directories(root);

  size_t i = 0;

  for (; i != dataset.train_entries().size(); i++) {
    string filename = create_filename(i, dataset.train_entries()[i].label());

    save_matrix(dataset.train_entries()[i].image(), root / filename);
  }

  for (size_t j = 0; j != dataset.test_entries().size(); j++, i++) {
    string filename = create_filename(i, dataset.test_entries()[j].label());

    save_matrix(dataset.test_entries()[j].image(), root / filename);
  }
}
