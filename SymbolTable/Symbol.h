#pragma once

#include <string>
#include <StringSymbol.h>
#include <Position.h>

namespace SymbolTable { 

class Symbol {
public:
  Symbol(std::string name, Position& position);
  Symbol(StringSymbol* name, Position& position);

  Symbol( const Symbol& ) = delete;
  void operator=( const Symbol& ) = delete;

  bool operator ==( const Symbol& a );
  bool operator !=( const Symbol& a );

  virtual ~Symbol() {};

  const StringSymbol* GetName() const { return name; }
  const Position& GetPosition() const { return position; }

protected:
  const StringSymbol* name;
  const Position position;

};

}
