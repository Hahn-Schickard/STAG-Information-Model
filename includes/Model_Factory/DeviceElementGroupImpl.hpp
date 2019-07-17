#ifndef _DEVICE_ELEMENT_GROUP_HPP
#define _DEVICE_ELEMENT_GROUP_HPP

#include "DeviceElement.hpp"
#include "DeviceElementGroup.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Model_Factory {

class DeviceElementGroupImpl : public Information_Model::DeviceElementGroup {
private:
  std::vector<std::shared_ptr<Information_Model::DeviceElement>> subelements;

public:
  DeviceElementGroupImpl(const std::string refId, const std::string name,
                         const std::string desc);

  std::string addDeviceEelment(const std::string name, const std::string desc,
                               Information_Model::ElementType type);

  std::vector<std::shared_ptr<Information_Model::DeviceElement>>
  getSubelements();
  Information_Model::DeviceElement *getSubelement(const std::string refId);
};
}

#endif //_DEVICE_ELEMENT_GROUP_HPP