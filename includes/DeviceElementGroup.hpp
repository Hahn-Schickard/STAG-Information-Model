#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP

#include "DeviceElement.hpp"
#include "NamedElement.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Information_Model {
class DeviceElementGroup : public DeviceElement {
protected:
  DeviceElementGroup(const std::string &ref_id, const std::string &name,
                     const std::string &desc)
      : DeviceElement(ref_id, name, desc, ElementType::GROUP) {}

public:
  using DeviceElements = std::vector<DeviceElementPtr>;

  virtual DeviceElements getSubelements() {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelements");
  }

  virtual DeviceElementPtr getSubelement(const std::string & /*ref_id*/) {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelement");
  }

  virtual ~DeviceElementGroup() = default;
};

using DeviceElementGroupPtr = std::shared_ptr<DeviceElementGroup>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP