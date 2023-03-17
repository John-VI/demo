// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clktextgrid.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "clktextureman.h"

using namespace clk;

textgrid::textgrid(texturehandle font, int wid, int hei, const GLint unimodel)
    : font(std::move(font)), intwid(wid), inthei(hei), unimodel(unimodel) {}

void textgrid::draw() const {
  if (text.empty())
    return;

  int maxchars = intwid / 8;
  int maxrows = inthei / 16;

  int maxout;

  if (maxchars * maxrows <= text.size())
    maxout = maxchars;
  else
    maxout = text.size();

  glm::mat4 model = glm::mat4(1.0f);

  font.enable();

  for (int i = 0; i < maxout; i++) {
    font.setSprite(0, text[i] - 32);

    glUniformMatrix4fv(
        unimodel, 1, GL_FALSE,
        glm::value_ptr(glm::scale(
            glm::translate(model, glm::vec3(4 + (i % maxchars) * 8.0f,
                                            8 + (i / maxchars) * 16.0f, 0.0f)),
            glm::vec3(8.0f, 16.0f, 1.0f))));

    glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);
  }
}