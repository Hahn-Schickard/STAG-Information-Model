#include "DeviceBuilder.hpp"
#include "Utils.hpp"
#include <memory>

using namespace std;
using namespace Model_Factory;

DeviceBuilder::DeviceBuilder(const std::string NAME, const std::string REF_ID,
                             const std::string DESC) {
  device = auto_ptr<iDevice>(new Device(REF_ID, NAME, DESC));
}

string DeviceBuilder::addDeviceElementGroup(const std::string NAME,
                                            const std::string DESC) {
  Device *device_impl = dynamic_cast<Device *>(device);
  string REF_ID = device_impl->addDeviceElementGroup(NAME, DESC);

  return REF_ID;
}