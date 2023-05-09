// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#include "messaging.h"

#include "clktex.h"

#include <iostream>
#include <stdexcept>

#define FONTHEI 16

messages::messages(clk::window &window, clk::sprite &fontface, devlevel level)
    : win(window), font(fontface), level(level) {}

std::deque<message> messages::queue;

void messages::push(message msg) {
  queue.push_back(msg);
  if (queue.size() > msgcap) {
    queue.pop_front();
  }
  std::cout << msg.text << std::endl;
}

void messages::draw() {
  int rows = win.getviewport(vports::LEFT).h / FONTHEI;
  for (int i = queue.size() - 1; i >= 0 && (queue.size() - i) < rows + 1; i--) {
    font.drawstring(vports::LEFT, 0, (rows - (queue.size() - i)) * FONTHEI,
                    queue.at(i).text);
  }
}
