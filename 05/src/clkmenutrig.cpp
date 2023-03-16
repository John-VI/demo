// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkmenutrig.h"

#include <stdexcept>
#include <unordered_map>
// #include <vector>

#include "clkkeybind.h"
#include "clkviewport.h"

#include "messaging.h"

clk::menubuild::menubuildktrig::menubuildktrig(menubuild &binding, double *val)
    : binding(binding), val(val) {}

void clk::menubuild::menubuildktrig::trigger(const SDL_Event &e) {
  binding.ktrigger(e, val);
}

clk::menubuild::menubuildktrig::~menubuildktrig() {}

clk::menubuild::menubuild(std::unordered_map<SDL_Keycode, double *> m,
                          inputman &iman, keybind &kman, sprite &f)
    : mappings{m}, mmanager(iman), kmanager(kman), font(f) {
  mmanblock = std::make_unique<menubuildmtrig>(*this);
  for (const auto &pair : mappings)
    kmanblocks.push_back(std::make_shared<menubuildktrig>(*this, pair.second));
  kmanagerreg();
}

void clk::menubuild::ktrigger(const SDL_Event &e, double *val) {
  target = val;
  mmanagerreg();
}

void clk::menubuild::kmanagerreg() {
  if (kbound) {
    // messages::push({"Already registered with manager.", severitylevel::DEV,
    //                 devlevel::DEV, 0});
    return;
  }
  kmanager.registerinput((SDL_Keycode)pair.first,
                         new menubuildktrig(*this, pair.second));
  kbound = true;
}

void clk::menubuild::kmanagerdereg() {
  if (kbound) {
    for (const auto &[key, _] : mappings)
      kmanager.deregister(key);
    kbound = false;
  }
}

clk::menubuild::menubuildmtrig::menubuildmtrig(menubuild &binding)
    : binding(binding) {}
clk::menubuild::menubuildmtrig::~menubuildmtrig() = default;

void clk::menubuild::menubuildmtrig::trigger(const SDL_Event &e) {
  binding.mtrigger(e);
}

void clk::menubuild::mmanagerreg() {
  kmanager.managerdereg();
  registration = mmanager.registerinput(SDL_KEYDOWN, new menubuildmtrig(*this));
}

void clk::menubuild::mmanagerdereg() {
  mmanager.deregister(SDL_KEYDOWN, registration);
  kmanager.managerreg(&mmanager);
}

void clk::menubuild::draw() {}

void clk::menubuild::mtrigger(const SDL_Event &e) {
  switch (e.key.keysym.sym) {
  case SDLK_BACKSPACE:
    buffer.pop_back();
    break;
  case SDLK_ESCAPE:
    mmanagerdereg();
    buffer.clear();
    target = nullptr;
    break;
  case SDLK_RETURN:
    *target = std::stod(buffer);
    buffer.clear();
    target = nullptr;
    mmanagerdereg();
    break;
  default:
    buffer.push_back(e.key.keysym.sym);
  }
}

clk::menubuild::~menubuild() {
  mmanagerdereg();
  kmanagerdereg();
}
