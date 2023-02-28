// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkviewport.h"

clk::viewport::viewport() : SDL_Rect{0, 0, 0, 0} {}
clk::viewport::viewport(int x, int y, int w, int h) : SDL_Rect{x, y, w, h} {}
