#ifndef _DEVICE_HPP
#define _DEVICE_HPP

#include <memory>
#include <string>

#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"
#include "Utils.hpp"
#include "iDevice.hpp"

namespace Model_Factory {
using namespace Information_Model;

class Device : public iDevice {
private:
  std::unique_ptr<iDeviceElementGroup> device_element_group;

public:
  Device(const std::string REF_ID, const std::string NAME,
         const std::string DESC)
      : iDevice(REF_ID, NAME, DESC) {}

  std::string addDeviceElementGroup(const std::string NAME,
                                    const std::string DESC);
  iDeviceElementGroup *getDeviceElementGroup();
};
}

#endif //_DEVICE_HPP
