#include "DeviceElementGroup.hpp"

using namespace std;

DeviceElementGroup::DeviceElementGroup(string refId, string name, string desc)
    : DeviceElement(refId, name, desc, ElementType::Group){};

iDeviceElement *DeviceElementGroup::getSubelement(const std::string refId)
{
  for (auto subelement : subelements)
  {
    if (subelement->getElementRefId() == refId)
    {
      return subelement;
    }
  }
  return nullptr;
}

vector<iDeviceElement *> DeviceElementGroup::getSubelements()
{
  return subelements;
}