#pragma once

#include <string>

namespace SymbolTable {

	class Position {
	public:
		Position(int _x, int _y);
		std::string ToString() const;
  private:
    int x;
    int y;
	};
}
