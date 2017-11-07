#pragma once

#include <string>
#include "Position.h"

namespace SymbolTable {
  std::string Position::ToString() const {
      return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
  }
}