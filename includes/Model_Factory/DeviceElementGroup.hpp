#ifndef _DEVICE_ELEMENT_GROUP_HPP
#define _DEVICE_ELEMENT_GROUP_HPP

#include "DeviceElement.hpp"
#include "iDeviceElementGroup.hpp"
#include <string>
#include <vector>

namespace Model_Factory {
using namespace Information_Model;

class DeviceElementGroup : public iDeviceElementGroup, public DeviceElement {
private:
  std::vector<iDeviceElement *> subelements;

public:
  DeviceElementGroup(const std::string refId, const std::string name,
                     const std::string desc);

  std::string addDeviceEelment(const std::string name, const std::string desc,
                               ElementType type);

  std::vector<iDeviceElement *> getSubelements();
  iDeviceElement *getSubelement(const std::string refId);
};
}

#endif //_DEVICE_ELEMENT_GROUP_HPP