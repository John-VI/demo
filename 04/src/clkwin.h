// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <array>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "clkviewport.h"

// typedef struct SDL_Color SDL_Color;
// typedef struct SDL_Window SDL_Window;
// typedef struct SDL_Renderer SDL_Renderer;

// void SDL_DestroyRenderer(SDL_Renderer *);
// void SDL_DestroyWindow(SDL_Window *);

namespace clk {
class window {
  const SDL_Color *color;

  SDL_GLContext glcontext;
  SDL_Window *win;

  std::array<viewport, (int)vports::MAX> viewports;

public:
  window(const char name[], int width, int height, const SDL_Color *color);
  
  void setcolor(const SDL_Color *newcolor);
  SDL_Window *getSDL_Window();
  SDL_GLContext *getSDL_GLContext();

  const viewport &getviewport(vports port);
  void setviewport(vports port, viewport newport);

  void clear();
  void draw();

  ~window();
};
} // namespace clk
