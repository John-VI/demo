// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <array>
#include <unordered_map>
#include <vector>
#include <string>

#include <GL/glew.h>

namespace clk {

enum class textureid { UNDEF, FROG, READ, FONT, MAX };

struct texture;
struct texturehandle;

class textureman {
  friend class texturehandle;

public:
  textureman(GLint unisection);

  texturehandle requestTexture(textureid);
  inline bool hasTexture(textureid id) { return textures.contains((int)id); }
  void enableTexture(const texturehandle &);
  void setSprite(const texturehandle &, int sheet, int sprite);

  texturehandle requestTexture(const char *filename);

  static constexpr GLfloat defaultsection[4] = {0.0f, 0.0f, 1.0f, 1.0f};

protected:
  texture *getTexture(int id);
  texture loadTexture(textureid);
  inline bool hasTexture(int id) { return textures.contains(id); }
  void cull(int id);

  std::unordered_map<int, texture> textures;
  //std::unordered_map<std::string, texture> extextures; // External textures
  textureid activetex;
  const GLint unisection;

  static const std::array<const char *, (int)textureid::MAX> texfilenames;
};

struct sheetinfo {
  int xoffset, yoffset;
  int xpadding, ypadding;
  int w, h;
  int count, cols;
};

struct texture {
  texture(GLuint tex, const int id, int w, int h);

  texture &operator=(texture &&other);
  texture(texture &&other);

  texture &operator=(const texture &other) = delete;
  texture(const texture &other) = delete;

  int refs = 0;
  GLuint tex;
  const int id;
  int culling = 0;
  bool live = true;

  std::vector<sheetinfo> spritesheets;

  inline int getw() const { return w; }
  inline int geth() const { return h; }

  ~texture();

private:
  int w, h;
};

struct texturehandle {
  friend class textureman;

  texturehandle(texture &, textureman *);

  texturehandle &operator=(texturehandle &&other);
  texturehandle(texturehandle &&other);

  // texturehandle &operator=(const texturehandle &other);
  // texturehandle(const texturehandle &other);

  inline bool live() const { return man->hasTexture((textureid)ref); }
  inline void enable() const { man->enableTexture(*this); }
  inline void setSprite(int sheet, int frame) const {
    man->setSprite(*this, sheet, frame);
  }

  inline void decrefcount() {
    if (man && --man->getTexture(ref)->refs <= 0)
      man->cull(ref);
  }

  ~texturehandle();

private:
  int ref;
  //  std::string name;

  textureman *man = nullptr;
};

} // namespace clk
