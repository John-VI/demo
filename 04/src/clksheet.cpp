// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clksheet.h"

#include <vector>

#include <SDL2/SDL_rect.h>

#include "clktex.h"

namespace clk {

inline int sheet::calcfx(const frameinfo &f, int frame) {
  return (frame % (f.r * f.c)) % f.c * (f.w + f.xpad) + f.x;
}

inline int sheet::calcfy(const frameinfo &f, int frame) {
  return (frame % (f.r * f.c)) / f.c * (f.h + f.ypad) + f.y;
}

sheet::sheet(clk::window &renderer, const char filename[],
             std::vector<frameinfo> frames)
    : sprite(renderer, filename), frames(frames) {}

void sheet::drawframe(const viewport &port, int id, int frame, int x, int y,
                      double angle, SDL_RendererFlip flip,
                      const SDL_Rect *dest) {
  const frameinfo &f = frames.at(id);

  SDL_Rect region = {calcfx(f, frame), calcfy(f, frame), f.w, f.h};

  // This function renders about the center.

  sprite::drawframe(port, x - f.w / 2, y - f.h / 2, region, dest, angle, flip);
}

void sheet::drawframe(vports port, int id, int frame, int x, int y,
                      double angle, SDL_RendererFlip flip,
                      const SDL_Rect *dest) {
  sheet::drawframe(renderer.getviewport(port), id, frame, x, y, angle, flip,
                   dest);
}

const frameinfo &sheet::getframe(int id) const { return frames[id]; }
} // namespace clk
