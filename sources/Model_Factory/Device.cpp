#include "Device.hpp"
#include <memory>

using namespace std;
using namespace Model_Factory;

string Device::addDeviceElementGroup(const std::string NAME,
                                     const std::string DESC) {
  const string REF_ID = generate_Reference_ID(
      dynamic_cast<Information_Model::DeviceElement *>(this));

  unique_ptr<Information_Model::iDeviceElementGroup> device_element_group_ptr(
      new DeviceElementGroup(REF_ID, NAME, DESC));

  device_element_group = move(device_element_group_ptr);

  return REF_ID;
}

Information_Model::iDeviceElementGroup *Device::getDeviceElementGroup() {
  return device_element_group.get();
}