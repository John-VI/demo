// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "clkinputman.h"

#include <stdexcept>

#include <SDL2/SDL_events.h>

#include <vector>
#include <stdbool.h>

clk::inputman::inputman() = default;

int
clk::inputman::processinputs() {
  int i = 0;
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    std::list<std::weak_ptr<inputtrigger>> *registered =
      &imap[(SDL_EventType)e.type];
    
    registered->remove_if([](auto t) { return t.expired(); });
    
    for (auto &t : *registered)
      (*t.lock())(e);
    
    i++;
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

void
clk::inputman::registerinput(SDL_EventType type,
                                 std::weak_ptr<inputtrigger> newtrigger) {
  imap[type].push_back(newtrigger);
}

void
clk::inputman::deregister(SDL_EventType type, const inputtrigger *trig) {
  imap[type].
    remove_if([trig](auto t) { return t.expired() || t.lock().get() == trig; });
}
