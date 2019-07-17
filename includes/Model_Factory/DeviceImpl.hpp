#ifndef _DEVICE_HPP
#define _DEVICE_HPP

#include <memory>
#include <string>

#include "Device.hpp"
#include "DeviceElementGroupImpl.hpp"
#include "NamedElement.hpp"
#include "Utils.hpp"

namespace Model_Factory {
using namespace Information_Model;

class DeviceImpl : public Device {
private:
  std::unique_ptr<DeviceElementGroup> device_element_group;

public:
  DeviceImpl(const std::string REF_ID, const std::string NAME,
             const std::string DESC)
      : Device(REF_ID, NAME, DESC) {}

  std::string addDeviceElementGroup(const std::string NAME,
                                    const std::string DESC);
  DeviceElementGroup *getDeviceElementGroup();
};
}

#endif //_DEVICE_HPP
