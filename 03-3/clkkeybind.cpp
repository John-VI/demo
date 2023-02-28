// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkkeybind.h"

#include <stdexcept>
#include <unordered_map>

#include "messaging.h"

clk::keybind::kbdbtrig::kbdbtrig(keybind &bind) : binding(bind) {}

void clk::keybind::kbdbtrig::trigger(const SDL_Event &e) { binding.trigger(e); }

clk::keybind::keybind() = default;

void clk::keybind::trigger(const SDL_Event &e) {
  try {
    if (registrations.at(e.key.keysym.scancode).expired())
      registrations.erase(e.key.keysym.scancode);
    else
      registrations.at(e.key.keysym.scancode).lock()->trigger(e);
  } catch (std::out_of_range) {
  }
}

void clk::keybind::managerreg(inputman *man) {
  if (manager) {
    messages::push({"Already registered with manager.", severitylevel::DEV,
                    devlevel::DEV, 0});
    return;
  }
  manager = man;
  regblock = std::make_shared<kbdbtrig>(*this);
  registration[0] = manager->registerinput(SDL_KEYDOWN, regblock);
  registration[1] = manager->registerinput(SDL_KEYUP, regblock);
}

void clk::keybind::managerdereg() {
  if (manager) {
    manager->deregister(SDL_KEYDOWN, registration[0]);
    manager->deregister(SDL_KEYDOWN, registration[1]);
    manager = nullptr;
  }
}

void clk::keybind::registerinput(SDL_Keycode code,
                                 std::weak_ptr<inputtrigger> newtrigger) {
  if (registrations[code].expired())
    registrations[code] = newtrigger;
  else
    throw std::runtime_error("Key is already bound.");
}

void clk::keybind::deregister(SDL_Keycode code) { registrations.erase(code); }
