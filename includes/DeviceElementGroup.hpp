#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {
struct DeviceElement;
inline bool operator==(
    const DeviceElement& lhs, const DeviceElement& rhs) noexcept;
inline bool operator!=(
    const DeviceElement& lhs, const DeviceElement& rhs) noexcept;

using DeviceElementPtr = std::shared_ptr<DeviceElement>;
using NonemptyDeviceElementPtr = NonemptyPointer::NonemptyPtr<DeviceElementPtr>;

/**
 * @addtogroup GroupModeling Device Element Group Modelling
 * @{
 */

struct DeviceElementNotFound : public std::runtime_error {
  DeviceElementNotFound(const std::string& ref_id)
      : std::runtime_error(
            "DeviceElement with reference id " + ref_id + " was not found") {}
};
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

  /**
   * @brief Returns a vector of contained elements ordered by DeviceElement
   * reference ids in an ascending order
   *
   * @return DeviceElements
   */
  virtual DeviceElements getSubelements() const {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelements");
  }

  /**
   * @brief Searches and returns a DeviceElement that matches a given reference
   * id
   *
   * @throws DeviceElementNotFound if no DeviceElement with given ref_id exists
   * within this group
   *
   */
  virtual NonemptyDeviceElementPtr getSubelement(
      const std::string& /*ref_id*/) const {
    throw std::runtime_error(
        "Called base implementation of DeviceElements::getSubelement");
  }

  virtual ~DeviceElementGroup() = default;

  bool operator==(const DeviceElementGroup& other) const noexcept {
    auto result = false;
    try {
      auto this_elements = getSubelements();
      auto other_elements = other.getSubelements();
      result = (this_elements.size() == other_elements.size());
      if (result) {
        // if groups have the same number of elements
        if (this_elements.size() > 0) {
          // if groups have elements
          for (size_t i = 0; i < this_elements.size(); ++i) {
            const auto& this_element = *(this_elements[i].get());
            const auto& other_element = *(other_elements[i].get());
            result = result && (this_element == other_element);
          }
        }
      }
    } catch (...) {
      result = false;
    }
    return result;
  }

  bool operator!=(const DeviceElementGroup& other) const noexcept {
    return !operator==(other);
  }

protected:
  DeviceElementGroup() = default;
};

using DeviceElementGroupPtr = std::shared_ptr<DeviceElementGroup>;
using NonemptyDeviceElementGroupPtr =
    NonemptyPointer::NonemptyPtr<DeviceElementGroupPtr>;
/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_GROUP_HPP