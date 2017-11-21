#include "VariableInfo.h"
#include <string>
#include <assert.h>

namespace SymbolTable {

TypeInfo::TypeInfo(T_VariableType type, const std::string userClass) : type(type)
{
    if(type == VT_UserClass) {
        this->userClass = StringSymbol::GetIntern(userClass);
    }
}

const std::string TypeInfo::GetTypeString() const
{
    switch (this->type) {
    case VT_Int:
        return "int";
    case VT_Boolean:
        return "boolean";
    case VT_IntArray:
        return "int[]";
    case VT_UserClass:
        return this->userClass->GetString();
    default:
        assert(false);
    }
}

TypeInfo::TypeInfo(T_VariableType type, const StringSymbol* userClass) : type(type)
{
    if(type == VT_UserClass) {
        this->userClass = userClass;
    }
}

const StringSymbol *TypeInfo::GetUserClass() const
{
    assert(type == VT_UserClass);
    return userClass;
}

bool TypeInfo::operator==(const TypeInfo &other) const
{
    if(other.GetType() != VT_UserClass) {
        return this->GetType() == other.GetType();
    }
    return this->GetType() == other.GetType() &&
            this->GetUserClass() == other.GetUserClass();
}

/////////////////////////////////////////////////////////////////////////////////////////

const TypeInfo& VariableInfo::GetType() const
{
    return type;
}

VariableInfo::VariableInfo(std::string _name, const Position  _position, TypeInfo _type ) :
    Symbol(_name, _position),
    type( _type )
{
}

}
