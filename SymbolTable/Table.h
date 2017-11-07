#pragma once

#include "Position.h"
#include "Symbol.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace SymbolTable {

	class Table {
	public:
		Table();	
		void beginBlock(const Position position);
		void endBlock(const Position position);
		// указатель?
		void addSymbol(const std::string src, const Symbol * symbol, const Position position); // может кинуть ошибку повторного объявления символа
		const Symbol * getInfo(const std::string src, const Position position) const; // может кинуть ошибку необявленной переменной
	private:
        std::vector< std::unordered_map<std::string, const Symbol*> > blockTable;
		int currentTable;
	};
}	
