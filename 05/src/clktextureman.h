// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <unordered_map>
#include <array>

#include <GL/glew.h>

namespace clk {

enum class textureid { UNDEF, FROG, READ, MAX };

struct texture;
struct texturehandle;

class textureman {
  friend class texturehandle;

public:
  textureman();

  texturehandle requestTexture(textureid);
  inline bool hasTexture(textureid id) { return textures.contains((int)id); }
  void enableTexture(const texturehandle &);

protected:
  texture *getTexture(int id);
  texture loadTexture(textureid);
  inline bool hasTexture(int id) { return textures.contains(id); }
  void cull(int id);

  std::unordered_map<int, texture> textures;
  textureid activetex;

  static const std::array<const char *, (int)textureid::MAX> texfilenames;
};


struct texture {
  texture(GLuint tex, const int id);
  texture &operator=(texture &&other);
  texture(texture &&other);

  texture &operator=(const texture &other) = delete;
  texture(const texture &other) = delete;

  int refs = 0;
  GLuint tex;
  const int id;
  int culling = 0;
  bool live = true;

  ~texture();
};


struct texturehandle {
  friend class textureman;

  texturehandle(texture &, textureman *);

  texturehandle &operator=(texturehandle &&other);
  texturehandle(texturehandle &&other);

  //texturehandle &operator=(const texturehandle &other);
  //texturehandle(const texturehandle &other);

  inline bool live() const { return man->hasTexture((textureid)ref); }
  inline void enable() { man->enableTexture(*this); }

  inline void decrefcount() {
    if (man && --man->getTexture(ref)->refs <= 0)
      man->cull(ref);
  }

  ~texturehandle();

private:
  int ref;
  textureman *man = nullptr;
};

}
