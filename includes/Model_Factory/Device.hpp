#ifndef _DEVICE_HPP
#define _DEVICE_HPP

#include <string>
#include <vector>

#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"
#include "iDevice.hpp"

namespace Model_Factory {
using namespace Information_Model;

class Device : public iDevice, public NamedElement {
protected:
  iDeviceElementGroup *device_element_group;

  Device(const std::string refId, const std::string name,
         const std::string desc);

public:
  iDeviceElementGroup *getDeviceElementGroup();
};
}

#endif //_DEVICE_HPP
