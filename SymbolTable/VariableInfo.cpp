#include "VariableInfo.h"
#include <string>
#include <assert.h>

namespace SymbolTable {

TypeInfo::TypeInfo(T_VariableType type, const std::string& userClass) : type(type)
{
    if(type == VT_UserClass) {
        this->userClass = StringSymbol::GetIntern(userClass);
    }
}

TypeInfo::TypeInfo(T_VariableType type, const StringSymbol* userClass) : type(type)
{
    if(type == VT_UserClass) {
        this->userClass = userClass;
    }
}

const StringSymbol *TypeInfo::GetUserClass()
{
    assert(type == VT_UserClass);
    return userClass;
}

/////////////////////////////////////////////////////////////////////////////////////////

const TypeInfo& VariableInfo::GetType() const
{
    return type;
}

VariableInfo::VariableInfo(std::string _name, Position  _position, TypeInfo _type ) :
    Symbol(_name, _position),
    type( _type )
{
}

}
