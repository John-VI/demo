// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <random>

namespace clk {

class randctl {
public:
  static void randinit();
  template <class T> static T randint(T from, T to);

private:
  static std::default_random_engine generator;
};

template <class T> T clk::randctl::randint(T from, T to) {
  std::uniform_int_distribution<T> distribution(from, to);
  return distribution(generator);
}
} // namespace clk
