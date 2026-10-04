#include <format>
#include <iostream>

#include "args.hpp"

#include "layers/linear.hpp"

#include "layers/activation_functions/leaky_relu.hpp"
#include "layers/activation_functions/sigmoid.hpp"
#include "layers/activation_functions/tanh.hpp"

#include "layers/uniform_noise.hpp"

#include "error_functions/cross_entropy_softmax.hpp"
#include "error_functions/mean.hpp"
#include "error_functions/mean_squared.hpp"

#include "optimizers/adam.hpp"
#include "optimizers/momentum.hpp"
#include "optimizers/sgd.hpp"

#include "neural_network_builder.hpp"

#include "mnist/load_dataset.hpp"
#include "src/mnist/dump_dataset.hpp"

#include "utils.hpp"

using namespace std;

const size_t SAMPLES_BETWEEN_LOG = 1000;

NeuralNetwork create_neural_network(bool train, bool train_with_noise) {
  auto cross_entropy_softmax = make_shared<CrossEntropySoftmax>();

  NeuralNetworkBuilder neural_network_builder(cross_entropy_softmax);

  if (train && train_with_noise)
    neural_network_builder.add_layer(make_shared<UniformNoise>(0.2, 0.0, 1.0));

  neural_network_builder.add_layer(make_shared<Linear>(784, 500));
  neural_network_builder.add_layer(make_shared<Sigmoid>());

  if (train && train_with_noise)
    neural_network_builder.add_layer(make_shared<UniformNoise>(0.2, 0.0, 1.0));

  neural_network_builder.add_layer(make_shared<Linear>(500, 300));
  neural_network_builder.add_layer(make_shared<Sigmoid>());

  if (train && train_with_noise)
    neural_network_builder.add_layer(make_shared<UniformNoise>(0.2, 0.0, 1.0));

  neural_network_builder.add_layer(make_shared<Linear>(300, 10));
  neural_network_builder.add_layer(cross_entropy_softmax);

  return neural_network_builder.build();
}

TFloat compute_test_accuracy(NeuralNetwork &neural_network,
                             const mnist::Dataset &dataset) {
  size_t correctly_predicted = 0;

  for (const mnist::DatasetEntry &entry : dataset.test_entries()) {
    Matrix output = neural_network.forward(entry.image());
    size_t digit = max_item_index(output);

    if (digit == entry.label()) {
      correctly_predicted++;
    }
  }

  return static_cast<TFloat>(correctly_predicted) /
         dataset.test_entries().size();
}

void showcase(NeuralNetwork &neural_network) {
  Matrix input(28 * 28, 1);

  do {
    load_matrix(input, "image.bin");

    Matrix output = neural_network.forward(input);
    size_t digit = max_item_index(output);

    for (size_t j = 0; j != output.size(); j++) {
      string s = format("{}: {:.4f}", j, output(j));

      if (j == digit) {
        s = format("\x1B[107m\x1B[30m{}\033[0m\033[0m", s);
      }

      cout << s << endl;
    }
  } while (cin.ignore(numeric_limits<streamsize>::max(), '\n') && !cin.eof());
}

int main(int argc, char **argv) {
  Args args = parse_args(argc, argv);

  NeuralNetwork neural_network =
      create_neural_network(args.train, args.train_with_noise);

  if (args.showcase || (args.train && args.train_existing_model) ||
      args.compute_test_accuracy) {
    load_matrix(neural_network.parameters(), args.model.value());
  }

  if (args.showcase) {
    showcase(neural_network);

    return 0;
  }

  if (!args.compute_test_accuracy && !args.showcase && !args.train &&
      !args.dump_dataset.has_value()) {
    return 1;
  }

  auto dataset = mnist::load_dataset(args.dataset.value());

  if (args.dump_dataset.has_value()) {
    dump_dataset(dataset, args.dump_dataset.value());

    return 0;
  }

  if (args.compute_test_accuracy) {
    cout << format("Test set accuracy: {}\n",
                   compute_test_accuracy(neural_network, dataset));

    return 0;
  }

  if (!args.train_existing_model) {
    neural_network.randomize_parameters(args.random_parameter_min,
                                        args.random_parameter_max);
  }

  std::unique_ptr<Optimizer> optimizer;

  if (args.optimizer == "SGD") {
    optimizer = make_unique<SGD>(neural_network, args.learning_rate);
  } else if (args.optimizer == "Adam") {
    optimizer = make_unique<Adam>(neural_network, args.beta1, args.beta2);
  } else if (args.optimizer == "Momentum") {
    optimizer =
        make_unique<Momentum>(neural_network, args.learning_rate, args.beta1);
  } else {
    return 1;
  }

  TFloat mean_error = 0.0;
  size_t correctly_predicted = 0;

  for (size_t j = 0;; j++) {
    if (j != 0 && j % SAMPLES_BETWEEN_LOG == 0) {
      cout << format("Samples: {:6}; epoch: {:6}; accuracy: "
                     "{:.4f}; mean error: {:.7f}\n",
                     j, j / dataset.train_entries().size(),
                     static_cast<TFloat>(correctly_predicted) /
                         SAMPLES_BETWEEN_LOG,
                     mean_error / SAMPLES_BETWEEN_LOG);

      correctly_predicted = 0;
      mean_error = 0.0;
    }

    if (j != 0 && j % 30000 == 0) {
      save_matrix(neural_network.parameters(), args.model.value());

      cout << "Parameters saved\n";
    }

    const mnist::DatasetEntry &entry =
        dataset.train_entries()[j % dataset.train_entries().size()];

    Matrix output = neural_network.forward(entry.image());
    size_t digit = max_item_index(output);

    if (digit == entry.label()) {
      correctly_predicted++;
    }

    Matrix expected_output(10, 1);
    expected_output.zeroize();
    expected_output(entry.label()) = 1.0;

    mean_error += neural_network.expect(std::move(expected_output));

    neural_network.backward();

    optimizer->step();

    neural_network.gradient().zeroize();
  }
}
