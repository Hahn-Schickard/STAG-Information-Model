#include "deviceElementGroup.hpp"

using namespace std;

DeviceElementGroup::DeviceElementGroup(string refId, string name, string desc)
    : DeviceElement::DeviceElement(refId, name, desc) {
  elementType = ElementType::Group;
};

DeviceElement *DeviceElementGroup::GetElementByRefId(std::string refId) {
  auto elem = std::find_if(
      subElements.begin(), subElements.end(),
      [refId](DeviceElement obj) { return obj.getReferenceId() == refId; });
  if (elem != subElements.end()) {
    return &(*elem);
  }
  return &(*elem);
}

vector<DeviceElement> DeviceElementGroup::getSubElements() {
  return subElements;
}

void DeviceElementGroup::addElement(DeviceElement &element) {
  subElements.push_back(element);
}