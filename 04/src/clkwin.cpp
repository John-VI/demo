// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include <SDL2/SDL_video.h>
#include <memory>
#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>

#include "clkwin.h"

#include "clkviewport.h"

using namespace clk;

clk::window::window(const char name[], int width, int height,
                    const SDL_Color *color)
  : color(color) {

  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  win =
      SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (win == nullptr)
    throw std::runtime_error(
        std::string({"Failed to initialize SDL window: ", SDL_GetError()}));

  glcontext = SDL_GL_CreateContext(win);

  if (glcontext == NULL)
    throw std::runtime_error(
        std::string({"Failed to initialize GL context: ", SDL_GetError()}));
  setcolor(color);

  glEnable(GL_TEXTURE_2D);
}

clk::window::~window() {
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(win);
}

void clk::window::setcolor(const SDL_Color *newcolor) {
  color = newcolor;
}

SDL_Window *clk::window::getSDL_Window() { return win; }

SDL_GLContext *clk::window::getSDL_GLContext() { return &glcontext; }

void clk::window::clear() {
  glClearColor(color->r / 255.0f, color->g / 255.0f, color->b / 255.0f, color->a / 255.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void clk::window::draw() { SDL_GL_SwapWindow(win); }

const viewport &clk::window::getviewport(vports port) {
  return viewports.at((int)port);
}

void clk::window::setviewport(vports port, viewport newport) {
  viewports.at((int)port) = newport;
}
