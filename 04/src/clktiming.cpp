// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clktiming.h"

#include <SDL2/SDL.h>

#include <SDL2/SDL_timer.h>
#include <bitset>

void clk::timer::start() {
  baseticks = SDL_GetTicks();
  pauseticks = 0;
  flags[0] = true;
  flags[1] = false;
}

void clk::timer::stop() {
  baseticks = 0;
  flags[0] = false;
  flags[1] = false;
}

void clk::timer::pause() {
  if (!flags[0] || flags[1])
    return;
  pauseticks = SDL_GetTicks() - baseticks;
  flags[1] = true;
}

void clk::timer::play() {
  if (!flags[0] || !flags[1])
    return;
  flags[1] = false;
  baseticks = SDL_GetTicks();
}

Uint32 clk::timer::ticks() const {
  if (!flags[0])
    return 0;
  else if (flags[1])
    return pauseticks;
  else
    return SDL_GetTicks() - baseticks + pauseticks;
}
