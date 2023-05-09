// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <SDL2/SDL_events.h>

#include <functional>

namespace clk {

// class inputtrigger {
// public:
//   virtual void trigger(const SDL_Event &e) = 0;
//   virtual ~inputtrigger();
// };

  typedef std::function<void(const SDL_Event &)> inputtrigger;

} // namespace clk
