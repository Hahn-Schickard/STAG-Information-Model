#ifndef _I_DEVICE_ELEMENT_HPP
#define _I_DEVICE_ELEMENT_HPP

#include "iNamedElement.hpp"

enum ElementType
{
    Undefined,
    Group,
    Readonly,
    Observable,
    Writable,
    Function
};

class iDeviceElement : iNamedElement
{
public:
    virtual ElementType getElementType() = 0;

protected:
    iDeviceElement();
};

#endif //_I_DEVICE_ELEMENT_HPP