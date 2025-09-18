#ifndef __STAG_INFORMATION_MODEL_DEVICE_HPP
#define __STAG_INFORMATION_MODEL_DEVICE_HPP

#include "Element.hpp"
#include "Group.hpp"
#include "MetaInfo.hpp"

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
 * Contains all of the modeled functionality of a single sensor/actor system
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilder
 */
struct Device : virtual public MetaInfo {
  ~Device() override = default;

  /**
   * @brief Returns the root group element
   *
   * @return GroupPtr
   */
  virtual GroupPtr group() const = 0;

  /**
   * @brief Returns the number of stored elements within the root Group
   *
   * @attention This value will always be larger than 0
   *
   * @return size_t
   */
  virtual size_t size() const = 0;

  /**
   * @brief Searches and returns an Element that matches a given reference
   * id
   *
   * @throws ElementNotFound - if no Element with given ref_id
   * exists within this group
   *
   */
  virtual ElementPtr element(const std::string& ref_id) const = 0;

  /**
   * @brief Uses the given Visitor callable, to visit each contained element
   * within the root group in no particular order
   *
   * @param visitor
   */
  virtual void visit(const Group::Visitor& visitor) const = 0;
};

using DevicePtr = std::shared_ptr<Device>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_DEVICE_HPP