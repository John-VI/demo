// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clktextureman.h"

#include <array>
#include <iostream>
#include <unordered_map>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_surface.h>

namespace clk {

const std::array<const char *, (int)textureid::MAX> textureman::texfilenames{
    {"undef.png", "frog.png", "reading.png", "compac.png"}};

texturehandle::texturehandle(texture &t, textureman *m) : man(m) {
  t.refs++;
  ref = t.id;
}

texturehandle::~texturehandle() {
  decrefcount();
  std::cout << "Called texturehandle destructor.\n";
}

texturehandle &texturehandle::operator=(texturehandle &&other) {
  if (this != &other) {
    decrefcount();

    std::swap(ref, other.ref);
    man = other.man;
    other.man = nullptr;
  }

  return *this;
}

texturehandle::texturehandle(texturehandle &&other) {
  *this = std::move(other);
}

/*
texturehandle &texturehandle::operator=(const texturehandle &other) {
  decrefcount();

  ref = other.ref;
  man = other.man;

  man->getTexture(ref)->refs++;

  return *this;
}

texturehandle::texturehandle(const texturehandle &other) {
  ref = other.ref;
  man = other.man;

  man->getTexture(ref)->refs++;
}
*/

texture::texture(GLuint tex, const int id, int w, int h)
    : tex(tex), id(id), w(w), h(h){};

texture &texture::operator=(texture &&other) {
  if (this != &other) {
    glDeleteTextures(1, &tex);
    std::swap(tex, other.tex);
    std::swap(refs, other.refs);
    std::swap(culling, other.culling);
    std::swap(w, other.w);
    std::swap(h, other.h);
    other.live = false;
  }

  return *this;
}

texture::texture(texture &&other) : id(other.id) { *this = std::move(other); }

texture::~texture() {
  if (live)
    glDeleteTextures(1, &tex);
  std::cout << "Texture destructor called. ID: " << id << ". Live? " << live
            << std::endl;
}

textureman::textureman(GLint unisection) : unisection(unisection) {
  textures.insert(std::pair<int, texture>((int)textureid::UNDEF,
                                          loadTexture(textureid::UNDEF)));
  textures.at((int)textureid::UNDEF).culling = 1;
  activetex = textureid::UNDEF;
}

texture *textureman::getTexture(int id) {
  if (!textures.contains(id))
    return nullptr;
  else
    return &textures.at(id);
}

texturehandle textureman::requestTexture(textureid id) {
  if (textures.contains((int)id))
    return texturehandle(textures.at((int)id), this);
  else {
    textures.insert(std::pair<int, texture>((int)id, loadTexture(id)));
    activetex = id;
    return texturehandle(textures.at((int)id), this);
  }
}

texture textureman::loadTexture(textureid id) {
  SDL_Surface *preconvsurf = IMG_Load(texfilenames[(int)id]);

  if (preconvsurf == NULL) {
    std::cerr << "Failure to load " << texfilenames[(int)id] << std::endl
              << IMG_GetError() << std::endl;
    throw std::runtime_error(IMG_GetError());
  }

  SDL_Surface *convsurf =
      SDL_ConvertSurfaceFormat(preconvsurf, SDL_PIXELFORMAT_RGBA32, 0);

  GLuint gltex;
  glGenTextures(1, &gltex);
  glBindTexture(GL_TEXTURE_2D, gltex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convsurf->w, convsurf->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, convsurf->pixels);

  int w = convsurf->w;
  int h = convsurf->h;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  SDL_FreeSurface(preconvsurf);
  SDL_FreeSurface(convsurf);

  return texture(gltex, (int)id, w, h);
}

void textureman::enableTexture(const texturehandle &handle) {
  int tex;

  if (handle.live())
    tex = handle.ref;
  else
    tex = (int)textureid::UNDEF;

  glBindTexture(GL_TEXTURE_2D, textures.at((int)handle.ref).tex);
  activetex = (textureid)tex;
  glUniform4fv(unisection, 1, defaultsection);
}

void textureman::setSprite(const texturehandle &handle, int sheetid, int sprite) {
  if (handle.ref != (int)activetex || !handle.live())
    return;

  const texture &tex = textures.at((int)handle.ref);

  const sheetinfo *sheet;

  try {
    sheet = &tex.spritesheets.at(sheetid);
  } catch (std::out_of_range) {
    glUniform4fv(unisection, 1, defaultsection);
    return;
  }

  sprite = sprite % sheet->count;

  GLfloat section[4] = {(sheet->xoffset + sheet->w * (sprite % sheet->cols) +
                         sheet->xpadding * (sprite % sheet->cols + 1)) /
                            tex.getw(),
                        (sheet->yoffset + sheet->h * (sprite / sheet->cols) +
                         sheet->ypadding * (sprite / sheet->cols + 1)) /
                            tex.geth(),
                        sheet->w / tex.getw(), sheet->h / tex.geth()};

  glUniform4fv(unisection, 1, section);
}

void textureman::cull(int id) {

  // TODO: Reenable culling after testing.

  // if (textures.at(id).refs <= 0 && textures.at(id).culling == 0) {
  // textures.erase(id);

  // if ((int)activetex == id)
  // enableTexture(texturehandle(textures.at((int)textureid::UNDEF), *this));
  //}
}

} // namespace clk
