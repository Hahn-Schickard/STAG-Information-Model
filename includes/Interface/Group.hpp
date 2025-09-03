#ifndef __STAG_INFORMATION_MODEL_GROUP_HPP
#define __STAG_INFORMATION_MODEL_GROUP_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace Information_Model {
struct Element;
using ElementPtr = std::shared_ptr<Element>;

/**
 * @addtogroup GroupModeling Device Element Group Modelling
 * @{
 */

struct ElementNotFound : public std::runtime_error {
  explicit ElementNotFound(const std::string& ref_id)
      : std::runtime_error(
            "Element with reference id " + ref_id + " was not found") {}
};

struct IDPointsThisGroup : public std::logic_error {
  explicit IDPointsThisGroup(const std::string& ref_id)
      : std::logic_error(
            "Reference ID " + ref_id + " points to this group element") {}
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
struct Group {
  using Elements = std::vector<ElementPtr>;

  /**
   * @brief Returns a vector of contained elements ordered by DeviceElement
   * reference ids in an ascending order
   *
   * @return DeviceElements
   */
  virtual Elements all() const = 0;

  /**
   * @brief Searches and returns a DeviceElement that matches a given reference
   * id
   *
   * @throws ElementNotFound - if no DeviceElement with given ref_id
   * exists within this group
   *
   */
  virtual ElementPtr element(const std::string& ref_id) const = 0;

  virtual ~Group() = default;
};

using GroupPtr = std::shared_ptr<Group>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_GROUP_HPP