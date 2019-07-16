#include "Device.hpp"
#include <memory>

using namespace std;
using namespace Model_Factory;

Device::Device(string refId, string name, string desc)
    : NamedElement(refId, name, desc), device_element_group(nullptr) {}

string Device::addDeviceElementGroup(const std::string NAME,
                                     const std::string DESC) {
  string REF_ID = generate_Reference_ID(auto_ptr<iNamedElement>(this));
  device_element_group = auto_ptr<iDeviceElementGroup>(
      new DeviceElementGroup((const string)REF_ID, NAME, DESC));
  return REF_ID;
}

iDeviceElementGroup *Device::getDeviceElementGroup() {
  return device_element_group;
}