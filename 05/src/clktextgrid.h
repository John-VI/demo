// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <GL/glew.h>

#include <vector>

#include "clktextureman.h"

namespace clk {

class textgrid {

public:
  textgrid(texturehandle gridfont, int scrwidth, int scrheight,
           const GLint unimodel);

  std::vector<char> text;

  void setDimensions(int newwidth, int newheight);
  void draw() const;

private:
  int intwid, inthei;

  texturehandle font;

  const GLint unimodel;
};
