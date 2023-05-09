// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkkeybind.h"

#include <stdexcept>
#include <unordered_map>

//#include <iostream>

#include "messaging.h"

// clk::keybind::kbdbtrig::kbdbtrig(keybind &bind) : binding(bind) {}

// void clk::keybind::kbdbtrig::trigger(const SDL_Event &e) { binding.trigger(e); }

clk::keybind::keybind() = default;

void clk::keybind::trigger(const SDL_Event &e) {
  std::list<std::weak_ptr<inputtrigger>> *registered =
    &registrations[(SDL_EventType)e.key.keysym.sym];

  //std::cout << "Got an input " << registered->size() << std::endl;

  registered->remove_if([](auto t) { return t.expired(); });
  for (auto &t : *registered)
    (*t.lock())(e);
}

void clk::keybind::managerreg(inputman *man) {
  if (manager) {
    // messages::push({"Already registered with manager.", severitylevel::DEV,
    //                 devlevel::DEV, 0});
    return;
  }
  manager = man;
  regblock = std::make_shared<inputtrigger>([this] (auto e) {this->trigger(e);});
  manager->registerinput(SDL_KEYDOWN, regblock);
  manager->registerinput(SDL_KEYUP, regblock);
}

void clk::keybind::managerdereg() {
  if (manager) {
    manager->deregister(SDL_KEYDOWN, regblock.get());
    manager->deregister(SDL_KEYDOWN, regblock.get());
    manager = nullptr;
  }
}

void clk::keybind::registerinput(SDL_Keycode code,
                                 std::weak_ptr<inputtrigger> newtrigger) {
  registrations[code].push_back(newtrigger);
}

void clk::keybind::deregister(SDL_Keycode code, const inputtrigger *trig) {
  registrations[code].
    remove_if([trig] (auto t) { return t.expired() || t.lock().get() == trig; });
}
