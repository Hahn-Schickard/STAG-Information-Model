#ifndef __STAG_INFORMATION_MODEL_GROUP_HPP
#define __STAG_INFORMATION_MODEL_GROUP_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
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
 * @brief An interface to Group element
 *
 * Groups other modeling entities together
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilder::addGroup(...) methods
 */
struct Group {
  using Visitor = std::function<void(const ElementPtr&)>;

  virtual ~Group() = default;

  /**
   * @brief Returns the number of stored elements within the Group
   *
   * @attention This value will always be larger than 0
   *
   * @return size_t
   */
  virtual size_t size() const = 0;

  /**
   * @brief Returns the stored elements as an unordered_map, where the key is
   * set to the relative path of the stored element, and the value is set to the
   * element itself
   *
   * @return std::unordered_map<std::string, ElementPtr>
   */
  virtual std::unordered_map<std::string, ElementPtr> asMap() const = 0;

  /**
   * @brief Returns a vector of contained elements ordered by Element ids in an
   * ascending order
   *
   * @return std::vector<ElementPtr>
   */
  virtual std::vector<ElementPtr> asVector() const = 0;

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
   * within this group in no particular order
   *
   * @param visitor
   */
  virtual void visit(const Visitor& visitor) const = 0;
};

using GroupPtr = std::shared_ptr<Group>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_GROUP_HPP