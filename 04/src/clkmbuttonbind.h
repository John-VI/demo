// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include "clkinputtrigger.h"

#include <memory>
#include <unordered_map>

namespace clk {

typedef struct inputman inputman;
typedef struct inputtrigger inputtrigger;

class mbuttonbind {
protected:
  struct mbuttontrig : public inputtrigger {
    mbuttonbind &binding;
    mbuttontrig(mbuttonbind &bind);
    void trigger(const SDL_Event &e) override;
    ~mbuttontrig();
  };
  int registration;
  inputman &manager;
  std::shared_ptr<mbuttontrig> manblock;

public:
  char stype, sfeat, sflag;

  mbuttonbind(inputman &);

  void trigger(const SDL_Event &e);
  void managerreg();
  void managerdereg();
  void registerinput(char, inputtrigger *newtrigger);
  void deregister(char);

  ~mbuttonbind();
};

} // namespace clk
