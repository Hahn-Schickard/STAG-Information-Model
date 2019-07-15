#ifndef _I_DEVICE_HPP
#define _I_DEVICE_HPP

#include "iNamedElement.hpp"
#include "iDeviceElementGroup.hpp"

class iDevice : iNamedElement
{
public:
    virtual iDeviceElementGroup *getDeviceElementGroup() = 0;

protected:
    iDevice();
};

#endif //_I_DEVICE_HPP