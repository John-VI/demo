// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include <SDL2/SDL_render.h>
#include <memory>
#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "clkwin.h"

#include "clktex.h"

clk::sprite::sprite(window &ren, const char filename[])
    : texture(nullptr, &SDL_DestroyTexture), renderer(ren) {
  SDL_Surface *surface = IMG_Load(filename);
  if (!surface)
    throw std::runtime_error(std::string(IMG_GetError()));

  int w, h;
  query(nullptr, nullptr, &w, &h);
  sheetoffset = {0, 0, w, h};
  screenoffset = sheetoffset;
}

clk::sprite::~sprite() = default;

int clk::sprite::query(uint32_t *format, int *access, int *w, int *h) const {
  int ret = SDL_QueryTexture(texture.get(), format, access, w, h);
  if (ret)
    throw std::runtime_error(std::string(SDL_GetError()));
  return ret;
}

SDL_Rect clk::sprite::draw(vports port, int x, int y) {
  const viewport &viewport = renderer.getviewport(port);
  SDL_Rect region = {screenoffset.x + x + viewport.x,
                     screenoffset.y + y + viewport.y, screenoffset.w,
                     screenoffset.h};
  SDL_RenderCopy(renderer.getSDL_Renderer(), texture.get(), &sheetoffset,
                 &region);
  return region;
}

SDL_Rect clk::sprite::draw(viewport &viewport, int x, int y) {
  SDL_Rect region = {screenoffset.x + x + viewport.x,
                     screenoffset.y + y + viewport.y, screenoffset.w,
                     screenoffset.h};
  SDL_RenderCopy(renderer.getSDL_Renderer(), texture.get(), &sheetoffset,
                 &region);
  return region;
}

void clk::sprite::setscreenoffset(SDL_Rect newoffset) {
  screenoffset = newoffset;
}

void clk::sprite::setsheetoffset(SDL_Rect newoffset) {
  sheetoffset = newoffset;
}

SDL_Rect clk::sprite::drawchar(vports port, int x, int y, char c) {
  const viewport &viewport = renderer.getviewport(port);
  SDL_Rect srcregion = {sheetoffset.x + (c - startchar) * sheetoffset.w,
                        sheetoffset.y, sheetoffset.w, sheetoffset.h};
  SDL_Rect dstregion = {screenoffset.x + x + viewport.x,
                        screenoffset.y + y + viewport.y, screenoffset.w,
                        screenoffset.h};
  SDL_RenderCopy(renderer.getSDL_Renderer(), texture.get(), &srcregion,
                 &dstregion);
  return dstregion;
}

SDL_Rect clk::sprite::drawchar(viewport &viewport, int x, int y, char c) {
  SDL_Rect srcregion = {sheetoffset.x + (c - startchar) * sheetoffset.w,
                        sheetoffset.y, sheetoffset.w, sheetoffset.h};
  SDL_Rect dstregion = {screenoffset.x + x + viewport.x,
                        screenoffset.y + y + viewport.y, screenoffset.w,
                        screenoffset.h};
  SDL_RenderCopy(renderer.getSDL_Renderer(), texture.get(), &srcregion,
                 &dstregion);
  return dstregion;
}

SDL_Rect clk::sprite::drawstring(vports port, int x, int y,
                                 const std::string &str) {
  for (int i = 0; i < str.length(); i++)
    drawchar(port, x + sheetoffset.w * i, y, str.at(i));

  return {x + screenoffset.x, y + screenoffset.y,
          screenoffset.w * (int)str.length(), screenoffset.h};
}

SDL_Rect clk::sprite::drawstring(viewport &viewport, int x, int y,
                                 const std::string &str) {
  for (int i = 0; i < str.length(); i++)
    drawchar(viewport, x + sheetoffset.w * i, y, str.at(i));

  return {x + screenoffset.x, y + screenoffset.y,
          screenoffset.w * (int)str.length(), screenoffset.h};
}

int clk::sprite::getw() const {
  int w;
  SDL_QueryTexture(texture.get(), nullptr, nullptr, &w, nullptr);
  return w;
}

int clk::sprite::geth() const {
  int h;
  SDL_QueryTexture(texture.get(), nullptr, nullptr, nullptr, &h);
  return h;
}

SDL_Rect clk::sprite::draw(vports port, int x, int y, double angle,
                           const SDL_Point *center, SDL_RendererFlip flip) {
  const viewport &viewport = renderer.getviewport(port);
  SDL_Rect region = {screenoffset.x + x + viewport.x,
                     screenoffset.y + y + viewport.y, screenoffset.w,
                     screenoffset.h};
  SDL_RenderCopyEx(renderer.getSDL_Renderer(), texture.get(), &sheetoffset,
                   &region, angle, center, flip);
  return region;
}

SDL_Rect clk::sprite::draw(viewport &port, int x, int y, double angle,
                           const SDL_Point *center, SDL_RendererFlip flip) {
  SDL_Rect region = {screenoffset.x + x + port.x, screenoffset.y + y + port.y,
                     screenoffset.w, screenoffset.h};
  SDL_RenderCopyEx(renderer.getSDL_Renderer(), texture.get(), &sheetoffset,
                   &region, angle, center, flip);
  return region;
}

SDL_Rect clk::sprite::drawframe(vports port, int x, int y,
                                const SDL_Rect &frame, const SDL_Rect *dest,
                                double angle, SDL_RendererFlip flip) {
  const viewport &viewport = renderer.getviewport(port);
  return drawframe(viewport, x, y, frame, dest, angle, flip);
}

SDL_Rect clk::sprite::drawframe(const viewport &port, int x, int y,
                                const SDL_Rect &frame, const SDL_Rect *dest,
                                double angle, SDL_RendererFlip flip) {
  SDL_Rect region;
  if (dest) {
    region.x = dest->x + x + port.x;
    region.y = dest->y + y + port.y;
    region.w = dest->w;
    region.h = dest->h;
  } else {
    region.x = x + port.x;
    region.y = y + port.y;
    region.w = frame.w;
    region.h = frame.h;
  }

  SDL_Point center = {frame.w / 2, frame.h / 2};

  SDL_RenderCopyEx(renderer.getSDL_Renderer(), texture.get(), &frame, &region,
                   angle, &center, flip);

  return region;
}

int clk::sprite::drawrect(const SDL_Rect *rect, const SDL_Color &color) {
  renderer.setcolor(&color);
  return SDL_RenderDrawRect(renderer.getSDL_Renderer(), rect);
  SDL_Color newcolor{0, 0, 0};
  renderer.setcolor(&newcolor);
}
