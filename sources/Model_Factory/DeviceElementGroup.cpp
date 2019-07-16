#include "DeviceElementGroup.hpp"
#include <memory>

using namespace std;
using namespace Model_Factory;

DeviceElementGroup::DeviceElementGroup(string refId, string name, string desc)
    : DeviceElement(refId, name, desc, ElementType::Group){};

string DeviceElementGroup::addDeviceEelment(const std::string name,
                                            const std::string desc,
                                            ElementType type) {
  iDeviceElement *subelement = new DeviceElement(name, desc, type);
  subelements.push_back(subelement);
}

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