#pragma once

#include <string>

namespace SymbolTable {

	class Position {
	public:
        Position();
		Position(int _x, int _y);
		std::string ToString() const;
	private:
		int x;
		int y;
		};
		
}
