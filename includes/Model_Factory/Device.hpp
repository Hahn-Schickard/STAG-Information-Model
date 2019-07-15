#ifndef _DEVICE_HPP
#define _DEVICE_HPP

#include <string>
#include <vector>

#include "iDevice.hpp"
#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"

class Device : public iDevice, public NamedElement
{
protected:
  iDeviceElementGroup *device_element_group;

  Device(const std::string refId, const std::string name, const std::string desc);

public:
  iDeviceElementGroup *getDeviceElementGroup();
};

#endif //_DEVICE_HPP
