#include "Device.hpp"

using namespace std;
using namespace Model_Factory;

Device::Device(string refId, string name, string desc)
    : NamedElement(refId, name, desc), device_element_group(nullptr) {}

iDeviceElementGroup *Device::getDeviceElementGroup() {
  return device_element_group;
}