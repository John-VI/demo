// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <SDL2/SDL_types.h>

#include <bitset>

namespace clk {

class timer {
public:
  void start();
  void stop();
  void pause();
  void play();

  Uint32 ticks() const;

private:
  Uint32 pauseticks = 0;
  Uint32 baseticks = 0;

  std::bitset<2> flags = 0;
  // started? | paused?
};

} // namespace clk
