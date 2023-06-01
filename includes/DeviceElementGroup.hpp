#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {
struct DeviceElement;
using DeviceElementPtr = std::shared_ptr<DeviceElement>;
using NonemptyDeviceElementPtr = NonemptyPointer::NonemptyPtr<DeviceElementPtr>;

/**
 * @addtogroup GroupModeling Device Element Group Modelling
 * @{
 */
/**
 * @brief An interface to DeviceElementGroup
 *
 * Groups other modeling entities together in a single collection
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addDeviceElementGroup()
 */
struct DeviceElementGroup {
  using DeviceElements = std::vector<NonemptyDeviceElementPtr>;

  virtual DeviceElements getSubelements() {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelements");
  }

  virtual DeviceElementPtr getSubelement(const std::string& /*ref_id*/) {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelement");
  }

  virtual ~DeviceElementGroup() = default;

protected:
  DeviceElementGroup() = default;
};

using DeviceElementGroupPtr = std::shared_ptr<DeviceElementGroup>;
using NonemptyDeviceElementGroupPtr =
    NonemptyPointer::NonemptyPtr<DeviceElementGroupPtr>;

/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP