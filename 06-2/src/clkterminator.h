// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include "clkinputtrigger.h"

#include "clkinputman.h"

namespace clk {

class terminator {
protected:
  int registration;
  inputman *manager = nullptr;
  std::shared_ptr<inputtrigger> registblock;
  bool finished;

public:
  terminator();
  void terminate();
  void managerreg(inputman *manager);
  void managerdereg();
  bool end();
};

} // namespace clk
