// © John Whitley

// #include "clkinputman.h"
// #include "clkterminator.h"

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>

#include <iostream>
#include <string>

#include "clkinputman.h"
#include "clkterminator.h"

const char *vertshadeglsl = R"glsl(
    #version 150 core

    in vec3 position;
    in vec4 color;

    out vec4 fragcolor;

    void main()
    {
        gl_Position = vec4(position, 1.0);
        fragcolor = color;
    }
)glsl";

const char *fragshadeglsl = R"glsl(
#version 150 core

in vec4 fragcolor;

out vec4 outColor;

void main()
{
    outColor = fragcolor;
})glsl";

const float vertices[] = {
  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
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

  positionattrib = glGetAttribLocation(shaderprogram, "position");
  glEnableVertexAttribArray(positionattrib);
  glVertexAttribPointer(positionattrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

  colorattrib = glGetAttribLocation(shaderprogram, "color");
  glEnableVertexAttribArray(colorattrib);
  glVertexAttribPointer(colorattrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

  SDL_GL_SetSwapInterval(1);

  clk::inputman iman;

  clk::terminator term;
  term.managerreg(&iman);

  while (!term.end()) {
    iman.processinputs();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderprogram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(sdlwin);
  }

  glDeleteProgram(shaderprogram);
  glDeleteShader(vertexshader);
  glDeleteShader(fragmentshader);

  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  SDL_DestroyWindow(sdlwin);

  return 0;
}
