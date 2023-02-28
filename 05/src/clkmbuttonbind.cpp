// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkmbuttonbind.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "clkinputman.h"

clk::mbuttonbind::mbuttontrig::mbuttontrig(mbuttonbind &bind) : binding(bind) {}
void clk::mbuttonbind::mbuttontrig::trigger(const SDL_Event &e) {
  binding.trigger(e);
}
clk::mbuttonbind::mbuttontrig::~mbuttontrig() = default;

clk::mbuttonbind::mbuttonbind(inputman &m) : manager(m) {
  manblock = std::make_shared<mbuttontrig>(*this);
}

void clk::mbuttonbind::managerreg() {
  registration = manager.registerinput(SDL_MOUSEBUTTONDOWN, manblock);
}

void clk::mbuttonbind::managerdereg() {
  manager.deregister(SDL_MOUSEBUTTONDOWN, registration);
}

clk::mbuttonbind::~mbuttonbind() { managerdereg(); }

void clk::mbuttonbind::trigger(const SDL_Event &e) {
  // tile *t;
  try {
    // t = g.gettile(e.button.x / 8, e.button.y / 16);
  } catch (std::runtime_error) {
    return;
  }

  switch (e.button.button) {
  case SDL_BUTTON_LEFT:
    // t->mktype((tiletype)stype);
    break;
  case SDL_BUTTON_RIGHT:
    // t->feat = (tilefeature)sfeat;
    break;
  case SDL_BUTTON_MIDDLE:
    // t->flags = sflag;
    break;
  }
}
