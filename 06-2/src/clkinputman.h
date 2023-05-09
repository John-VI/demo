// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <list>
#include <memory>
#include <unordered_map>
#include <utility>

#include <SDL2/SDL_events.h>

#include "clkinputtrigger.h"

namespace clk {

typedef std::unordered_map<SDL_EventType,
                           std::list<std::unique_ptr<inputtrigger>>>
    inputmap;
typedef std::unordered_map<
    SDL_EventType, std::list<std::weak_ptr<inputtrigger>>>
    idmap;

class inputman {
protected:
  idmap imap;
  // inputmap &sharedmap;

public:
  inputman();
  
  int
  processinputs();
  /*int registerinput(SDL_EventType type,
                    std::unique_ptr<inputtrigger> newtrigger); */
  void
  registerinput(SDL_EventType type, std::weak_ptr<inputtrigger> newtrigger);
  
  void
  deregister(SDL_EventType type, const inputtrigger *);
};

} // namespace clk
