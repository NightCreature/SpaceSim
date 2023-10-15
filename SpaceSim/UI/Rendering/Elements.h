#pragma once


namespace FE
{

namespace Rendering
{

struct EmptyElement {};

#define REGISTER_ELEMENT(ELEMENTTYPE) RegisterElement<ELEMENTTYPE>(#ELEMENTTYPE);

inline void RegisterComponents()
{
    REGISTER_ELEMENT(EmptyElement);
}

}

}