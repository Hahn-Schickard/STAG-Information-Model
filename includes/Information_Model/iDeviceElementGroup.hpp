#ifndef _I_DEVICE_ELEMENT_GROUP_HPP
#define _I_DEVICE_ELEMENT_GROUP_HPP

#include "iNamedElement.hpp"
#include "iDeviceElement.hpp"
#include <string>
#include <vector>

class iDeviceElementGroup : iDeviceElement
{
public:
    virtual std::vector<iDeviceElement *> getSubelements() = 0;
    virtual iDeviceElement *getSubelement(const std::string refId) = 0;

protected:
    iDeviceElementGroup();
};

#endif //_I_DEVICE_ELEMENT_GROUP_HPP