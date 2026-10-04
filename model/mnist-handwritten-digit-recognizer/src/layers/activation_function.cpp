#include "activation_function.hpp"

size_t ActivationFunction::parameters_count() const { return 0; }

void ActivationFunction::set_parameters(TFloat *) {}

void ActivationFunction::set_gradient(TFloat *) {}

void ActivationFunction::backward(const Matrix &) {}
