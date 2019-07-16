#ifndef _DEVICE_HPP
#define _DEVICE_HPP

#include <string>
#include <vector>

#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"
#include "Utils.hpp"
#include "iDevice.hpp"

namespace Model_Factory {
using namespace Information_Model;

class Device : public iDevice, public NamedElement {
private:
  iDeviceElementGroup *device_element_group;

public:
  Device(const std::string refId, const std::string name,
         const std::string desc);

  std::string addDeviceElementGroup(const std::string NAME,
                                    const std::string DESC);
  iDeviceElementGroup *getDeviceElementGroup();
};
}

#endif //_DEVICE_HPP
