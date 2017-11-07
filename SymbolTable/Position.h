#pragma once

#include <string>

namespace SymbolTable {

	class Position {
	public:
		std::string ToString() const;
  private:
    int x;
    int y;
	};
}