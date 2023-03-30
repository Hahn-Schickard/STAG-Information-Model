#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {
class DeviceElement;

/**
 * @brief An interface to DeviceElementGroup
 *
 * This class groups other modeling entities together under a single collection
 *
 * This class is implemented in Information Model Manager Project and is built
 * via DeviceBuilderInterface::addDeviceElementGroup() or
 * DeviceBuilderInterface::addDeviceElement() with type set to
 * ElementType::GROUP
 */
class DeviceElementGroup {
protected:
  DeviceElementGroup() = default;

public:
  /** aka std::vector<NonemptyDeviceElementPtr> */
  using DeviceElements =
      std::vector<NonemptyPointer::NonemptyPtr<std::shared_ptr<DeviceElement>>>;

  virtual DeviceElements getSubelements() {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelements");
  }

  virtual std::shared_ptr<DeviceElement> getSubelement(
      const std::string& /*ref_id*/) {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelement");
  }

  virtual ~DeviceElementGroup() = default;
};

using DeviceElementGroupPtr = std::shared_ptr<DeviceElementGroup>;
using NonemptyDeviceElementGroupPtr =
    NonemptyPointer::NonemptyPtr<DeviceElementGroupPtr>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP