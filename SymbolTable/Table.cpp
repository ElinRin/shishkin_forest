#pragma once

#include "Position.h"
#include "Symbol.h"
#include "DeclarationException.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace SymbolTable {
  Table::Table()
  {
    currentTable = 0;
  }

  void Table::beginBlock(const Position position)
  {
    blockTable.push_back();
    currentTable++;
  }
  void Table::endBlock(const Position position)
  {
    blockTable.pop_back();
    currentTable--;
  }

  void Table::addSymbol(const std::string src, const Symbol * symbol, const Position position)
  {
    auto element = blockTable[currentTable].find(src);
    if (elemet != blockTable[currentTable].end())
    {
      throw new DeclarationException(position.ToString() + " Variable already declared.\n");
    }
    else
    {
      blockTable[currentTable].insert( { src, symbol } );
    }
  }

  const Symbol * Table::getInfo(const std::string src, const Position position)
  {
    int pointer = currentTable;
    bool isFind = false;
    while (pointer >= 0 && !isFind)
    {
      auto element = blockTable[pointer].find(src);
      if (elemet != blockTable[pointer].end())
      {
        isFind = true;
        return element->second;
      }
      else
      {
        pointer--;
      }
    }

    if ( !isFind )
    {
      throw new DeclarationException(position.ToString() + " Variable not declared.\n");
    }
    
  }
}	