#ifndef _DEVICE_ELEMENT_GROUP_HPP
#define _DEVICE_ELEMENT_GROUP_HPP

#include "DeviceElement.hpp"
#include "iDeviceElementGroup.hpp"
#include <string>
#include <vector>

namespace Model_Factory {
using namespace Information_Model;

class DeviceElementGroup : public iDeviceElementGroup, public DeviceElement {
public:
  DeviceElementGroup(const std::string refId, const std::string name,
                     const std::string desc);

  std::vector<iDeviceElement *> getSubelements();
  iDeviceElement *getSubelement(const std::string refId);

private:
  std::vector<iDeviceElement *> subelements;
};
}

#endif //_DEVICE_ELEMENT_GROUP_HPP