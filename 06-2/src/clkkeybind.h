// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include "clkinputtrigger.h"

#include <unordered_map>
#include <list>

#include "clkinputman.h"

namespace clk {

  typedef std::unordered_map<SDL_Keycode, std::list<std::weak_ptr<inputtrigger>>> keymap;

class keybind {
protected:
  // struct kbdbtrig : public inputtrigger {
  //   keybind &binding;
  //   kbdbtrig(keybind &bind);
  //   void trigger(const SDL_Event &e) override;
  // };
  //int registration[2];
  std::shared_ptr<inputtrigger> regblock;
  inputman *manager = nullptr;
  keymap registrations;

public:
  keybind();
  //  keybind(
  //      std::unordered_map<SDL_Keycode, std::unique_ptr<inputtrigger>>
  //      basemap);
  void trigger(const SDL_Event &e);
  void managerreg(inputman *manager);
  void managerdereg();
  void registerinput(SDL_Keycode code, std::weak_ptr<inputtrigger> newtrigger);
  void deregister(SDL_Keycode code, const inputtrigger *);
};

} // namespace clk
