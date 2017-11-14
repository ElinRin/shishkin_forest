#ifndef DECLARATION_EXCEPTION_H
#define DECLARATION_EXCEPTION_H

#include <string>
#include "Position.h"

namespace SymbolTable {

class DeclarationException : public std::exception {
public:
    virtual char const* what() const noexcept override;
    DeclarationException( const std::string& message, const Position& position );
    virtual ~DeclarationException() throw() {}
private:
    std::string msgresult;
};

}

#endif
