// Copyright (c) John Allen Whitley, 2023, BSD 3-Clause

#include "player.h"

#include <stdio.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

const std::array<SDL_Keycode, 10> player::keys =
  { 'w', 'a', 's', 'd', SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_LCTRL, SDLK_SPACE };

player::player(glm::mat4 defaultview, GLint viewuniform) :
  vmatrix(defaultview), uniview(viewuniform) {

  movestate.fill(0);
}

void player::managerreg(clk::keybind *kbd) {
  kbdman = kbd;

  registration = std::make_shared<clk::inputtrigger>
    ([this] (auto e) { this->trigger(e); });

  for (auto c : keys)
    kbd->registerinput(c, registration);
}

void player::managerdereg() {
  for (auto c : keys)
    kbdman->deregister(c, registration.get());

  registration.reset();
}

void player::trigger(const SDL_Event &e) {
  int state = (e.key.state == SDL_PRESSED);
  
  switch (e.key.keysym.sym) {
  case 'w':
    movestate[2] = 1 * state;
    break;
  case 's':
    movestate[2] = -1 * state;
    break;
  case 'a':
    movestate[0] = 1 * state;
    break;
  case 'd':
    movestate[0] = -1 * state;
    break;
  case SDLK_SPACE:
    movestate[1] = -1 * state;
    break;
  case SDLK_LCTRL:
    movestate[1] = 1 * state;
    break;
  case SDLK_UP:
    movestate[4] = -1 * state;
    break;
  case SDLK_DOWN:
    movestate[4] = 1 * state;
    break;
  case SDLK_LEFT:
    movestate[3] = -1 * state;
    break;
  case SDLK_RIGHT:
    movestate[3] = 1 * state;
    break;
  }
}

void player::updateview() {
  vmatrix = glm::translate(vmatrix,
			   glm::vec3(movestate[0] * transrate,
				     movestate[1] * transrate,
				     movestate[2] * transrate));
  vmatrix = glm::rotate(vmatrix, rotrate * movestate[3], glm::vec3(0, 1, 0));
  vmatrix = glm::rotate(vmatrix, rotrate * movestate[4], glm::vec3(1, 0, 0));

  glUniformMatrix4fv(uniview, 1, GL_FALSE, glm::value_ptr(vmatrix));
}
