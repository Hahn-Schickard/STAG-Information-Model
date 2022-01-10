#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "NonemptyPtr.hpp"

namespace Information_Model {
class DeviceElement;

class DeviceElementGroup {
protected:
  DeviceElementGroup() = default;

public:
  using DeviceElements =
    std::vector<NonEmptyPtr<std::shared_ptr<DeviceElement>>>;

  virtual DeviceElements getSubelements() {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelements");
  }

  virtual std::shared_ptr<DeviceElement> getSubelement(
    const std::string & /*ref_id*/)
  {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelement");
  }

  virtual ~DeviceElementGroup() = default;
};

using DeviceElementGroupPtr = std::shared_ptr<DeviceElementGroup>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP