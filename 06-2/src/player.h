// Copyright (c) John Allen Whitley, 2023, BSD 3-Clause

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>
#include <array>

#include "clkinputtrigger.h"
#include "clkkeybind.h"

class player {
 private:
  std::shared_ptr<clk::inputtrigger> registration;
  clk::keybind *kbdman = nullptr;

  static const std::array<SDL_Keycode, 10> keys;

 protected:
  glm::mat4 vmatrix;
  GLint uniview;

  std::array<signed char, 5> movestate;
  static constexpr float transrate = .01;
  static constexpr float rotrate = glm::radians(1.0f);

 public:
  player(glm::mat4, GLint viewuniform);

  void managerreg(clk::keybind *);
  void managerdereg();

  void trigger(const SDL_Event &e);
  void updateview();
};
