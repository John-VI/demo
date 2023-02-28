// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkinputman.h"

#include <stdexcept>

#include <SDL2/SDL_events.h>

#include <vector>

clk::inputman::inputman() = default;

int clk::inputman::processinputs() {
  int i = 0;
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    std::list<std::weak_ptr<inputtrigger>> *registered = nullptr;
    try {
      registered = &imap.at((SDL_EventType)e.type).second;
    } catch (std::out_of_range) {
      return 0;
    }
    if (registered) {
      registered->remove_if([](auto t) { return t.expired(); });
      for (auto &t : *registered)
        t.lock()->trigger(e);
      i++;
    }
  }
  return i;
}

/* int clk::inputman::registerinput(SDL_EventType type,
                                 std::unique_ptr<inputtrigger> newtrigger) {
  int id = ++imap[type].first;
  newtrigger->id = id;
  imap[type].second.push_back(std::move(newtrigger));
  return id;
} */

int clk::inputman::registerinput(SDL_EventType type,
                                 std::weak_ptr<inputtrigger> newtrigger) {
  int id = ++imap[type].first;
  newtrigger.lock()->id = id;
  imap[type].second.push_back(newtrigger);
  return id;
}

void clk::inputman::deregister(SDL_EventType type, int id) {
  imap.at(type).second.remove_if(
      [id](auto t) { return t.expired() || t.lock()->id == id; });
}
