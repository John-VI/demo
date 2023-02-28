// © John Whitley

// #include "clkinputman.h"
// #include "clkterminator.h"

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/glu.h>

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
uniform bool warp;
uniform uint time;

void main()
{
    if (warp)
        outColor = texture(tex, vec2(tan(fragtexcoord.x * time * 0.0001f), tan(fragtexcoord.y * -time * 0.0001f))) * fragcolor;
    else
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

#define INTWID 640
#define INTHEI 640

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

  SDL_Window *sdlwin = SDL_CreateWindow("01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INTWID, INTHEI, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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
  GLint uniwarp = glGetUniformLocation(shaderprogram, "warp");
  GLint unitime = glGetUniformLocation(shaderprogram, "time");

  GLuint tex[2];
  glGenTextures(2, tex);

  glBindTexture(GL_TEXTURE_2D, tex[0]);

  SDL_Surface *surf = IMG_Load("frog.png");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);
  SDL_FreeSurface(surf);

  glBindTexture(GL_TEXTURE_2D, tex[1]);

  surf = IMG_Load("reading.png");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGB,
               GL_UNSIGNED_BYTE, surf->pixels);
  SDL_FreeSurface(surf);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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
  glm::mat4 perproj = glm::perspective<float>(glm::radians(90.0f), (float)INTWID / (float)INTHEI, 0.01f, 100.0f);
  // glm::mat4 ortproj = glm::ortho(0, 640, 480, 0);
  glm::mat4 ortproj = glm::ortho<float>(0.0f, INTWID, INTHEI, 0.0f);
  perproj = glm::translate(perproj, glm::vec3(0, 0, -5));
  glm::mat4 view = glm::mat4(1.0f);
  // view = glm::rotate(view, glm::radians(-20.f), glm::vec3(1.0f, 0.0f, 0.0f));
  // view = glm::rotate(view, glm::radians(-20.f), glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(uniproj, 1, GL_FALSE, glm::value_ptr(ortproj));
  glUniformMatrix4fv(uniview, 1, GL_FALSE, glm::value_ptr(view));
  glUniform1i(uniwarp, 1);
  glUniform1ui(unitime, 0);

  float x = 0.5;
  float y = 0.5;
  float z = 0.5;
  float dx = .002;
  float dy = .001;
  float dz = .003;
  Uint32 elapsedtime = 0;

  while (!term.end()) {
    framedelta.start();
    elapsedtime += mseconds;

    iman.processinputs();

    // if (x + .1 > 5.0f || x - .1 < -5.0f)
    //   dx = -dx;

    // if (y + .1 > 5.0f || y - .1 < -5.0f)
    //   dy = -dy;

    // if (z + .1 > 2.0f || z - .1 < -5.0f)
    //   dz = -dz;

    // x += dx * mseconds;
    // y += dy * mseconds;
    // z += dz * mseconds;
    // std::cout << z << std::endl;

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderprogram);

    glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, wVBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, webo);
    // glDrawElements(GL_TRIANGLES, 6 * 1, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glUniform1ui(unitime, elapsedtime);

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    trans = glm::scale(glm::mat4(1.0), glm::vec3(11, 11, 11));
    // trans = glm::rotate(trans, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // trans = glm::rotate(trans, glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(unimodel, 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(uniproj, 1, GL_FALSE, glm::value_ptr(perproj));
    glUniform1ui(uniwarp, true);

    glDrawElements(GL_TRIANGLES, 6 * 5, GL_UNSIGNED_INT, 0);

    glDisable(GL_DEPTH_TEST);

    trans = glm::translate(glm::mat4(1.0), glm::vec3(INTWID / 2, INTHEI / 2, 0));
    trans = glm::scale(trans, glm::vec3(INTWID - elapsedtime * 0.01, -INTHEI + elapsedtime * 0.01, 0));
    glUniformMatrix4fv(unimodel, 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(uniproj, 1, GL_FALSE, glm::value_ptr(ortproj));
    glUniform1ui(uniwarp, false);

    glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);

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
