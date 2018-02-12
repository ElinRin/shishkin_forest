#pragma once

#define BIND(Interface, Implemenation) template<> \
    Interface* Factory::Create<Interface>() \
    { return new Implemenation(); }
  
namespace Factory {

template <class T>
T* Create();

}
