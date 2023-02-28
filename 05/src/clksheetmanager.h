// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <list>
#include <memory>

#include "clksheet.h"

namespace clk {

class sheetmanager {
protected:
  std::list<std::weak_ptr<sheet>> sheets;
  bool loadsheet(int id);

public:
  std::shared_ptr<sprite> getsheet(int id);
};

} // namespace clk
