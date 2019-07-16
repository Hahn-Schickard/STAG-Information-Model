#include "DeviceElementGroup.hpp"

using namespace std;
using namespace Model_Factory;

DeviceElementGroup::DeviceElementGroup(string refId, string name, string desc)
    : DeviceElement(refId, name, desc, ElementType::Group){};

iDeviceElement *DeviceElementGroup::getSubelement(const std::string REF_ID) {
  for (auto subelement : subelements) {
    if (subelement->getElementRefId() == REF_ID) {
      return subelement;
    }
  }
  return nullptr;
}

vector<iDeviceElement *> DeviceElementGroup::getSubelements() {
  return subelements;
}