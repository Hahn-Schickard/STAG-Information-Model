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

  virtual DeviceElements getSubelements() const {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelements");
  }

  virtual DeviceElementPtr getSubelement(const std::string& /*ref_id*/) const {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelement");
  }

  virtual ~DeviceElementGroup() = default;

  bool operator==(const DeviceElementGroup& other) const {
    auto result = false;
    auto this_elements = getSubelements();
    auto other_elements = other.getSubelements();
    if (this_elements.size() == other_elements.size()) {
      if (this_elements.size() > 1) {
        for (size_t i = 0; i < this_elements.size(); ++i) {
          auto this_element = this_elements[i].get();
          auto other_element = other_elements[i].get();
          result = result && (this_element == other_element);
        }
      } else {
        // if both groups are empty
        result = true;
      }
    }
    return result;
  }

protected:
  DeviceElementGroup() = default;
};

using DeviceElementGroupPtr = std::shared_ptr<DeviceElementGroup>;
using NonemptyDeviceElementGroupPtr =
    NonemptyPointer::NonemptyPtr<DeviceElementGroupPtr>;

inline bool operator==(
    const DeviceElementGroupPtr& lhs, const DeviceElementGroupPtr& rhs) {
  return *lhs == *rhs;
}

/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP