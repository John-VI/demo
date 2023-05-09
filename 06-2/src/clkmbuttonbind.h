// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include "clkinputtrigger.h"
#include "clkinputman.h"

#include <memory>
#include <unordered_map>

namespace clk {

class mbuttonbind {
protected:
  int registration;
  inputman &manager;
  std::shared_ptr<inputtrigger> manblock;

public:
  char stype, sfeat, sflag;

  mbuttonbind(inputman &);

  void trigger(const SDL_Event &e);
  void managerreg();
  void managerdereg();
  void registerinput(char, inputtrigger *newtrigger);
  void deregister(char);

  ~mbuttonbind();
};

} // namespace clk
