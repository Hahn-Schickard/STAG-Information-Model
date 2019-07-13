#include "device.hpp"

using namespace std;

Device::Device(string refId, string name, string desc)
    : NamedElement(refId, name, desc) {
  device_element_groups = vector<DeviceElementGroup>();
}

void Device::addDeviceElementGroup(DeviceElementGroup *group) {
  device_element_groups.push_back(*group);
}

DeviceElementGroup *Device::getDeviceElementGroup() {

  return &device_element_groups[0];
}

DeviceElementGroup *Device::getDeviceElementGroup(string refId) {

  for (size_t i = 0; i < device_element_groups.size(); i++) {
    if (device_element_groups[i].getReferenceId() == refId) {
      return &device_element_groups[i];

    } else if (device_element_groups[i].getElementType() == Group) {
      return (DeviceElementGroup *)findSubElement(&device_element_groups[i],
                                                  refId);
    }
  }
  return nullptr;
}

DeviceElement *Device::findSubElement(DeviceElement *deviceElement,
                                      std::string refId) {
  if (deviceElement->getElementType() == ElementType::Group) {
    DeviceElementGroup *subGroup = (DeviceElementGroup *)deviceElement;
    vector<DeviceElement> subElements = subGroup->getSubElements();
    for (size_t j = 0; j < subElements.size(); j++) {
      if (subElements[j].getReferenceId() == refId) {
        return &subElements[j];
        break;
      } else if (subElements[j].getElementType() == Group) {
        return findSubElement(&subElements[j], refId);
      }
    }
  }
  return nullptr;
}

vector<DeviceElementGroup> Device::getDeviceElementGroups() {
  return device_element_groups;
}