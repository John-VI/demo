// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkrand.h"

#include <chrono>
#include <random>

std::default_random_engine clk::randctl::generator;

void clk::randctl::randinit() {
  generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}
