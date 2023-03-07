// © John Whitley

// #include "clkinputman.h"
// #include "clkterminator.h"

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "clkinputman.h"
#include "clkterminator.h"
#include "clktiming.h"

const char *vertshadeglsl = R"glsl(
    #version 150 core

    in vec3 position;
    in vec4 color;
    in vec2 texcoord;

    out vec4 fragcolor;
    out vec2 fragtexcoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main()
    {
        gl_Position = proj * view * model * vec4(position, 1.0);
        fragcolor = color;
        fragtexcoord = texcoord;
    }
)glsl";

const char *fragshadeglsl = R"glsl(
#version 150 core

in vec4 fragcolor;
in vec2 fragtexcoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
    outColor = texture(tex, fragtexcoord) * fragcolor;
})glsl";

const float vertices[] = {
    -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, 0.5f,   0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f,    0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f, -0.5f,   0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};

const GLuint elements[] = {
    0, 1, 2, 2, 3, 0,

    0, 1, 4, 1, 4, 5,

    2, 3, 6, 3, 6, 7,

    0, 3, 4, 3, 4, 7,

    1, 2, 5, 2, 5, 6,

    4, 5, 6, 6, 7, 4,
};

int main (int argc, char *argv[]) {
  GLuint programid = 0;
  GLint vertexpos = -1;
  GLuint VBO = 0;
  GLuint VAO = 0;
  GLuint vertexshader;
  GLuint fragmentshader;
  GLuint shaderprogram;
  GLint positionattrib;
  GLint colorattrib;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window *sdlwin = SDL_CreateWindow("01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  SDL_GLContext glcontext = SDL_GL_CreateContext(sdlwin);

  IMG_Init(IMG_INIT_PNG);

  glewExperimental = GL_TRUE;
  glewInit();

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);

  vertexshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexshader, 1, &vertshadeglsl, NULL);
  glCompileShader(vertexshader);

  fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentshader, 1, &fragshadeglsl, NULL);
  glCompileShader(fragmentshader);

  shaderprogram = glCreateProgram();
  glAttachShader(shaderprogram, vertexshader);
  glAttachShader(shaderprogram, fragmentshader);
  glBindFragDataLocation(shaderprogram, 0, "outcolor");
  glLinkProgram(shaderprogram);
  glUseProgram(shaderprogram);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  positionattrib = glGetAttribLocation(shaderprogram, "position");
  glEnableVertexAttribArray(positionattrib);
  glVertexAttribPointer(positionattrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);

  colorattrib = glGetAttribLocation(shaderprogram, "color");
  glEnableVertexAttribArray(colorattrib);
  glVertexAttribPointer(colorattrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

  GLint texcoordattrib = glGetAttribLocation(shaderprogram, "texcoord");
  glEnableVertexAttribArray(texcoordattrib);
  glVertexAttribPointer(texcoordattrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

  GLint unitexture = glGetUniformLocation(shaderprogram, "texture");
  GLint unimodel = glGetUniformLocation(shaderprogram, "model");
  GLint uniview = glGetUniformLocation(shaderprogram, "view");
  GLint uniproj = glGetUniformLocation(shaderprogram, "proj");

  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  SDL_Surface *surf = IMG_Load("frog.png");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);
  SDL_FreeSurface(surf);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // GLuint wVBO;
  // GLuint webo;

  // glGenBuffers(1, &wVBO);

  // glBindBuffer(GL_ARRAY_BUFFER, wVBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(wallverts), wallverts, GL_STATIC_DRAW);

  // glGenBuffers(1, &webo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, webo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallelems), wallelems,
  //              GL_STATIC_DRAW);

  SDL_GL_SetSwapInterval(1);

  clk::inputman iman;

  clk::terminator term;
  term.managerreg(&iman);

  clk::timer framedelta;
  Uint32 mseconds = 0;

  glm::mat4 trans = glm::mat4(1.0f);
  glm::mat4 proj = glm::perspective<float>(glm::radians(90.0f), 640.0f / 480.0f, 0.01f, 100.0f);
  proj = glm::translate(proj, glm::vec3(0, 0, -5));
  glm::mat4 view = glm::mat4(1.0f);
  // view = glm::rotate(view, glm::radians(-20.f), glm::vec3(1.0f, 0.0f, 0.0f));
  // view = glm::rotate(view, glm::radians(-20.f), glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(uniproj, 1, GL_FALSE, glm::value_ptr(proj));
  glUniformMatrix4fv(uniview, 1, GL_FALSE, glm::value_ptr(view));

  float x = 0.5;
  float y = 0.5;
  float z = 0.5;
  float dx = .002;
  float dy = .001;
  float dz = .003;

  while (!term.end()) {
    framedelta.start();

    iman.processinputs();

    if (x + .1 > 5.0f || x - .1 < -5.0f)
      dx = -dx;

    if (y + .1 > 5.0f || y - .1 < -5.0f)
      dy = -dy;

    if (z + .1 > 2.0f || z - .1 < -5.0f)
      dz = -dz;

    x += dx * mseconds;
    y += dy * mseconds;
    z += dz * mseconds;
    std::cout << z << std::endl;

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderprogram);

    glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, wVBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, webo);
    // glDrawElements(GL_TRIANGLES, 6 * 1, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    trans = glm::scale(glm::mat4(1.0), glm::vec3(11, 11, 11));
    // trans = glm::rotate(trans, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // trans = glm::rotate(trans, glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(unimodel, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6 * 5, GL_UNSIGNED_INT, 0);

    trans = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
    glUniformMatrix4fv(unimodel, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);

    glDisable(GL_DEPTH_TEST);

    SDL_GL_SwapWindow(sdlwin);

    mseconds = framedelta.ticks();
  }

  glDeleteProgram(shaderprogram);
  glDeleteShader(vertexshader);
  glDeleteShader(fragmentshader);

  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &VAO);

  SDL_DestroyWindow(sdlwin);

  return 0;
}
