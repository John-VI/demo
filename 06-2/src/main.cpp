// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include <cstdio>
#include <iostream>
#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "clkinputman.h"
#include "clkinputtrigger.h"
#include "clkkeybind.h"
#include "clkterminator.h"
#include "clktextgrid.h"
#include "clktextureman.h"
#include "clktiming.h"
#include "clkwin.h"

#define INTWID 640
#define INTHEI 640

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
	uniform vec4 section;

    void main()
    {
        gl_Position = proj * view * model * vec4(position, 1.0);
        fragcolor = color;
        fragtexcoord = vec2(texcoord.x * section.z + section.x, 
			texcoord.y * section.w + section.y);
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
uniform vec4 shade;

void main()
{
    if (warp)
        outColor = texture(tex, vec2(tan(fragtexcoord.x * time * 0.0001f), tan(fragtexcoord.y * -time * 0.0001f))) * fragcolor * shade;
    else
        outColor = texture(tex, fragtexcoord) * fragcolor * shade;
})glsl";

const float vertices[] = {
    -0.5f, 0.5f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.5f, 0.5f, 0.0f,
    1.0f,  1.0f, 1.0f, 1.0f,  1.0f,  0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0f,  1.0f, 1.0f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
};

const GLuint elements[] = {0, 1, 2, 0, 2, 3};

extern "C" void GLAPIENTRY messageCallback(GLenum source, GLenum type,
                                           GLuint id, GLenum severity,
                                           GLsizei length,
                                           const GLchar *message,
                                           const void *userParam) {
  fprintf(stderr, "GL %s 0x%x of severity 0x%x.\n%s\n\n",
          type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "Callback", type, severity,
          message);
}

// struct texktrig : public clk::inputtrigger {
//   clk::textureid tex = clk::textureid::UNDEF;

//   void trigger(const SDL_Event &e) override {
//     tex = (clk::textureid)((int)tex + 1);
//     if (tex >= clk::textureid::MAX)
//       tex = clk::textureid::UNDEF;

//     std::cout << "Cycle: " << (int)tex << std::endl;
//   }
// };

// struct typetrig : public clk::inputtrigger {
//   typetrig(clk::textgrid &g) : g(g) {}

//   void trigger(const SDL_Event &e) override {
//     g.text.push_back(e.key.keysym.sym);
//   }

//   clk::textgrid &g;
// };

int shaderError(GLuint shader) {
  GLint compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (compiled == GL_FALSE) {
    GLint maxlength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxlength);

    std::vector<GLchar> errorlog(maxlength);
    glGetShaderInfoLog(shader, maxlength, &maxlength, &errorlog[0]);

    printf("%s\n", errorlog.data());

    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  SDL_Color black = {0, 0, 255, 0};
  clk::window win("04", INTWID, INTHEI, &black);

  IMG_Init(IMG_INIT_PNG);

  // OpenGL initialization

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(messageCallback, 0);

  GLuint VBO = 0;
  GLuint VAO = 0;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);

  GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexshader, 1, &vertshadeglsl, NULL);
  glCompileShader(vertexshader);

  if (shaderError(vertexshader)) {
    glDeleteShader(vertexshader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    return 5;
  }

  GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentshader, 1, &fragshadeglsl, NULL);
  glCompileShader(fragmentshader);

  if (shaderError(vertexshader)) {
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    return 6;
  }

  GLuint shaderprogram = glCreateProgram();
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
               GL_STATIC_DRAW);

  GLint positionattrib = glGetAttribLocation(shaderprogram, "position");
  glEnableVertexAttribArray(positionattrib);
  glVertexAttribPointer(positionattrib, 3, GL_FLOAT, GL_FALSE,
                        9 * sizeof(float), 0);

  GLint colorattrib = glGetAttribLocation(shaderprogram, "color");
  glEnableVertexAttribArray(colorattrib);
  glVertexAttribPointer(colorattrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  GLint texcoordattrib = glGetAttribLocation(shaderprogram, "texcoord");
  glEnableVertexAttribArray(texcoordattrib);
  glVertexAttribPointer(texcoordattrib, 2, GL_FLOAT, GL_FALSE,
                        9 * sizeof(float), (void *)(7 * sizeof(float)));

  GLint unitexture = glGetUniformLocation(shaderprogram, "texture");
  GLint unimodel = glGetUniformLocation(shaderprogram, "model");
  GLint uniview = glGetUniformLocation(shaderprogram, "view");
  GLint uniproj = glGetUniformLocation(shaderprogram, "proj");
  GLint uniwarp = glGetUniformLocation(shaderprogram, "warp");
  GLint unitime = glGetUniformLocation(shaderprogram, "time");
  GLint unishade = glGetUniformLocation(shaderprogram, "shade");
  GLint unisection = glGetUniformLocation(shaderprogram, "section");

  GLfloat darkshade[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  glUniform4fv(unishade, 1, darkshade);

  clk::textureman texman(unisection);

  SDL_GL_SetSwapInterval(1);

  clk::inputman iman;

  clk::terminator term;
  term.managerreg(&iman);

  clk::keybind kbd;
  kbd.managerreg(&iman);

  // std::shared_ptr<texktrig> txk = std::make_unique<texktrig>();
  // kbd.registerinput(SDLK_0, txk);

  clk::timer framedelta;
  Uint32 mseconds = 0;

  glm::mat4 trans = glm::mat4(1.0f);
  glm::mat4 perproj = glm::perspective<float>(glm::radians(90.0f), (float)INTWID / (float)INTHEI, 0.01f, 100.0f);
  glm::mat4 ortproj = glm::ortho<float>(0.0f, INTWID, INTHEI, 0.0f, 0.0f, 1.0f);
  glm::mat4 view = glm::mat4(1.0f);

  glUniformMatrix4fv(uniview, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(uniproj, 1, GL_FALSE, glm::value_ptr(perproj));
  glUniform1i(uniwarp, 1);
  glUniform1ui(unitime, 0);

  std::cout << glm::to_string(ortproj) << std::endl;

  Uint32 elapsedtime = 0;

  std::unordered_map<clk::textureid, clk::texturehandle> handles;
  handles.insert(std::make_pair(clk::textureid::UNDEF,
                                texman.requestTexture(clk::textureid::UNDEF)));
  handles.insert(std::make_pair(clk::textureid::FROG,
                                texman.requestTexture(clk::textureid::FROG)));
  handles.insert(std::make_pair(clk::textureid::READ,
                                texman.requestTexture(clk::textureid::READ)));
  handles.insert(std::make_pair(clk::textureid::FONT,
                                texman.requestTexture(clk::textureid::FONT)));

  clk::textgrid tgrid(texman.requestTexture(clk::textureid::FONT), INTWID,
                      INTHEI, unimodel);

  tgrid.text.push_back('f');
  tgrid.text.push_back('u');
  tgrid.text.push_back('d');

  std::shared_ptr<clk::inputtrigger> turntrig = std::make_shared<clk::inputtrigger>([&view] (auto &e) {
    std::cout << "Move" << std::endl;
    if (e.key.keysym.sym == SDLK_LEFT)
      view = glm::rotate(view, glm::radians(.5f), glm::vec3(0, 1, 0));
    else
      view = glm::rotate(view, glm::radians(-.5f), glm::vec3(0, 1, 0));
      }
    );

  kbd.registerinput(SDLK_LEFT, turntrig);
  kbd.registerinput(SDLK_RIGHT, turntrig);

  while (!term.end()) {
    framedelta.start();
    elapsedtime += mseconds;

    iman.processinputs();

    glUseProgram(shaderprogram);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glUniform1ui(unitime, elapsedtime);

    texman.enableTexture(handles.at(clk::textureid::READ));

    glEnable(GL_DEPTH_TEST);

    win.clear();

    glUniformMatrix4fv(uniview, 1, GL_FALSE, glm::value_ptr(view));

    glUniformMatrix4fv(uniproj, 1, GL_FALSE, glm::value_ptr(perproj));
    // trans = glm::translate(glm::mat4(1.0f),
    //                         glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    trans = glm::scale(trans, glm::vec3(11.0f, 11.0f, 11.0f));
    glUniformMatrix4fv(unimodel, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform1ui(uniwarp, false);

    glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);

    trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f));
    trans = glm::scale(trans, glm::vec3(11.0f, 11.0f, 11.0f));
    glUniformMatrix4fv(unimodel, 1, GL_FALSE, glm::value_ptr(trans));

    glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);

    glDisable(GL_DEPTH_TEST);

    glUniformMatrix4fv(uniproj, 1, GL_FALSE, glm::value_ptr(ortproj));
    trans = glm::mat4(1.0);
    glUniformMatrix4fv(unimodel, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform1ui(uniwarp, false);

    glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);

    tgrid.draw();

    win.draw();

    mseconds = framedelta.ticks();
  }

  glDeleteProgram(shaderprogram);
  glDeleteShader(vertexshader);
  glDeleteShader(fragmentshader);

  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &VAO);

  return 0;
}
