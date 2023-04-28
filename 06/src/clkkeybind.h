// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include "clkinputtrigger.h"

#include <unordered_map>

#include "clkinputman.h"

namespace clk {

/* FIXME: Redo this to allow multiple binding like inputman but right know I
 * can't fing care */

class keybind {
protected:
  struct kbdbtrig : public inputtrigger {
    keybind &binding;
    kbdbtrig(keybind &bind);
    void trigger(const SDL_Event &e) override;
  };
  int registration[2];
  std::shared_ptr<kbdbtrig> regblock;
  inputman *manager = nullptr;
  std::unordered_map<SDL_Keycode, std::weak_ptr<inputtrigger>> registrations;

public:
  keybind();
  //  keybind(
  //      std::unordered_map<SDL_Keycode, std::unique_ptr<inputtrigger>>
  //      basemap);
  void trigger(const SDL_Event &e);
  void managerreg(inputman *manager);
  void managerdereg();
  void registerinput(SDL_Keycode code, std::weak_ptr<inputtrigger> newtrigger);
  void deregister(SDL_Keycode code);
};

} // namespace clk
