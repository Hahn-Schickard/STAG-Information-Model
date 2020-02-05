#ifndef __DATA_MODEL_DEVICE_ELEMENT_GROUP_HPP
#define __DATA_MODEL_DEVICE_ELEMENT_GROUP_HPP

#include "DeviceElement.hpp"
#include "NamedElement.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Information_Model {
class DeviceElementGroup : public DeviceElement {
protected:
  DeviceElementGroup(const std::string &REF_ID, const std::string &NAME,
                     const std::string &DESC)
      : DeviceElement(REF_ID, NAME, DESC, ElementType::GROUP) {}

public:
  virtual std::vector<std::shared_ptr<DeviceElement>> getSubelements() = 0;
  virtual std::shared_ptr<DeviceElement>
  getSubelement(const std::string &REF_ID) = 0;

  virtual ~DeviceElementGroup() = default;
};

} // namespace Information_Model

#endif //__DATA_MODEL_DEVICE_ELEMENT_GROUP_HPP