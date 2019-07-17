#include "DeviceBuilder.hpp"
#include "DeviceElementBuilder.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Model_Factory;

DeviceBuilder::DeviceBuilder(const std::string NAME, const std::string REF_ID,
                             const std::string DESC) {
  unique_ptr<Information_Model::iDevice> device_ptr(
      new Device(REF_ID, NAME, DESC));
  device = move(device_ptr);
}

string DeviceBuilder::addDeviceElementGroup(const std::string NAME,
                                            const std::string DESC) {
  Device *device_impl = dynamic_cast<Device *>(device.get());
  string REF_ID = device_impl->addDeviceElementGroup(NAME, DESC);

  return REF_ID;
}

string DeviceBuilder::addDeviceElement(const std::string NAME,
                                       const std::string DESC,
                                       Information_Model::ElementType type) {
  DeviceElementGroup *device_element_group =
      dynamic_cast<DeviceElementGroup *>(device->getDeviceElementGroup());
  const string REF_ID =
      device_element_group->addDeviceEelment(NAME, DESC, type);

  return REF_ID;
}