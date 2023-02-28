// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <SDL2/SDL_rect.h>

enum class vports : int { FULL, RIGHT, LEFT, CENTER, MAX };

namespace clk {
struct viewport : public SDL_Rect {
  viewport();
  viewport(int, int, int, int);
};
} // namespace clk
