// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <vector>

#include "clktextureman.h"

namespace clk {

struct frameinfo {
  int x;
  int y;
  int w;
  int h;
  int r;
  int c;
  int xpad;
  int ypad;

  inline explicit operator SDL_Rect() const {
    return {-(w / 2), -(h / 2), w, h};
  }
};

class sheet : public sprite {
private:
  texturehandle tex;

  inline int calcfx(const frameinfo &f, int frame);
  inline int calcfy(const frameinfo &f, int frame);

protected:
  std::vector<frameinfo> frames;

public:
  sheet(window &renderer, const char filename[], std::vector<frameinfo> frames);

  void drawframe(const viewport &port, int id, int frame, int x, int y,
                 double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE,
                 const SDL_Rect *dest = nullptr);
  void drawframe(vports port, int id, int frame, int x, int y, double angle = 0,
                 SDL_RendererFlip flip = SDL_FLIP_NONE,
                 const SDL_Rect *dest = nullptr);
  void setframeinfo(std::vector<frameinfo> frames);

  const frameinfo &getframe(int id) const;
};

} // namespace clk
