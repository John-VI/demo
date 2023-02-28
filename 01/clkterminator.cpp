// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkterminator.h"

#include <stdexcept>

clk::terminator::quittrig::quittrig(clk::terminator &bind) : binding(bind){};
void clk::terminator::quittrig::trigger(const SDL_Event &e) {
  binding.terminate();
}

clk::terminator::quittrig::~quittrig() {}

clk::terminator::terminator() {finished = false;};

void clk::terminator::managerreg(inputman *man) {
  if (manager) {
    return;
  }
  manager = man;
  registblock = std::make_shared<quittrig>(*this);
  registration = manager->registerinput(SDL_QUIT, registblock);
}

void clk::terminator::managerdereg() {
  manager->deregister(SDL_QUIT, registration);
  manager = nullptr;
}

void clk::terminator::terminate() { finished = 1; }

bool clk::terminator::end() { return finished; }
