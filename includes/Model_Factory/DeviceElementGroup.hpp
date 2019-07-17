#ifndef _DEVICE_ELEMENT_GROUP_HPP
#define _DEVICE_ELEMENT_GROUP_HPP

#include "DeviceElement.hpp"
#include "iDeviceElementGroup.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Model_Factory {

class DeviceElementGroup : public Information_Model::iDeviceElementGroup {
private:
  std::vector<std::unique_ptr<Information_Model::DeviceElement>> subelements;

public:
  DeviceElementGroup(const std::string refId, const std::string name,
                     const std::string desc);

  std::string addDeviceEelment(const std::string name, const std::string desc,
                               Information_Model::ElementType type);

  std::vector<std::unique_ptr<Information_Model::DeviceElement>> const &
  getSubelements();
  Information_Model::DeviceElement *getSubelement(const std::string refId);
};
}

#endif //_DEVICE_ELEMENT_GROUP_HPP