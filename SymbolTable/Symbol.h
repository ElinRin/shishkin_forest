#pragma once

#include <string>

namespace SymbolTable { 

  class Symbol {
  public:
    const std::string& String() const;
    Symbol() {};
    Symbol( const Symbol& ) = delete;
    void operator=( const Symbol& ) = delete;

    friend bool operator ==( const Symbol& a, const Symbol& b );
		friend bool operator !=( const Symbol& a, const Symbol& b );

    virtual ~Symbol();
  private:
    std::string name;
  };
}