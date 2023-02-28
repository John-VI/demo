// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <SDL2/SDL_events.h>

namespace clk {

class inputtrigger {
public:
  int id;
  virtual void trigger(const SDL_Event &e) = 0;
  virtual ~inputtrigger();
};

} // namespace clk