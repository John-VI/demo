// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include "clkwin.h"
#include <deque>
#include <string>

enum class devlevel { GAME, DEV };
enum class severitylevel { DEATH, CRITICAL, WARNING, NORMAL, DEV };

namespace clk {
typedef struct sprite sprite;
}

struct message {
  std::string text;
  severitylevel severity;
  devlevel dev;
  unsigned int turn;
};

class messages {
protected:
  static std::deque<message> queue;
  devlevel level;
  clk::window &win;
  clk::sprite &font;
  const static unsigned int msgcap = 500;

public:
  messages(clk::window &window, clk::sprite &fontface,
           devlevel level = devlevel::GAME);

  static void push(message);

  void draw();
};
