#ifndef _DEVICE_FACTORY_HPP
#define _DEVICE_FACTORY_HPP

#include "Device.hpp"
#include "DeviceElement.hpp"
#include "DeviceElementGroup.hpp"

namespace Model_Factory {

class DeviceBuilder {
private:
  iDevice *device;

public:
  DeviceBuilder();
  DeviceBuilder(const std::string NAME, const std::string REF_ID,
                const std::string DESC);

  std::string addDeviceElementGroup(const std::string NAME,
                                    const std::string DESC);
  std::string addDeviceElement(const std::string NAME, const std::string DESC,
                               ElementType type);
};
}

#endif //_DEVICE_FACTORY_HPP