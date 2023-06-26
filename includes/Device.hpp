#ifndef __INFORMATION_MODEL_DEVICE_HPP
#define __INFORMATION_MODEL_DEVICE_HPP

#include "DeviceElement.hpp"
#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"

#include <memory>
#include <stdexcept>
#include <string>

namespace Information_Model {
/**
 * @addtogroup DeviceModeling Device Modelling
 * @{
 */
/**
 * @brief An interface to the Device model
 *
 * Contains all of the modeled functionality of a singular addressable
 * sensor/actor system
 *
 * Each Device MUST have a reference ID (saved as ref_id), which will be used to
 * address any modeled functionality by Data Consumer Adapter Implementations
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface
 */
struct Device : public NamedElement {
  /**
   * @brief Returns the root DeviceElementGroup
   *
   * @return NonemptyDeviceElementGroupPtr
   */
  virtual NonemptyDeviceElementGroupPtr getDeviceElementGroup() const {
    throw std::runtime_error(
        "Called base implementation of Device::getDeviceElementGroup");
  }

  /**
   * @brief Searches and returns a DeviceElement that matches a given reference
   * id within this device
   *
   * @see DeviceElementGroup::getSubelement() for reference
   *
   * @throws DeviceElementNotFound if no DeviceElement with given ref_id exists
   *
   */
  virtual DeviceElementPtr getDeviceElement(
      const std::string& /* ref_id */) const {
    throw std::runtime_error(
        "Called base implementation of Device::getDeviceElement");
  }

  virtual ~Device() = default;

  bool operator==(const Device& other) const noexcept {
    try {
      auto result = NamedElement::operator==(other);
      if (result) {
        const auto& this_elements = *(getDeviceElementGroup().get());
        const auto& other_elements = *(other.getDeviceElementGroup().get());
        result = result && (this_elements == other_elements);
      }
      return result;
    } catch (...) {
      return false;
    }
  }

  bool operator!=(const Device& other) const noexcept {
    return !operator==(other);
  }

protected:
  Device(const std::string& ref_id,
      const std::string& name,
      const std::string& desc)
      : NamedElement(ref_id, name, desc) {}
};

using DevicePtr = std::shared_ptr<Device>;
using NonemptyDevicePtr = NonemptyPointer::NonemptyPtr<DevicePtr>;
/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_HPP