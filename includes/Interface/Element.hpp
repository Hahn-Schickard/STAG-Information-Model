#ifndef __STAG_INFORMATION_MODEL_ELEMENT_HPP
#define __STAG_INFORMATION_MODEL_ELEMENT_HPP

#include "Callable.hpp"
#include "Group.hpp"
#include "MetaInfo.hpp"
#include "Observable.hpp"
#include "Readable.hpp"
#include "Writable.hpp"

#include <memory>
#include <string>

namespace Information_Model {

/**
 * @addtogroup ElementModeling Device Element Modelling
 * @{
 */
/**
 * @enum ElementTypeEnum
 * @brief ElementType enumeration, specifying the available DeviceElement
 * types.
 *
 */
enum class ElementType : uint8_t {
  Group,
  Readable,
  Writable,
  Observable,
  Callable
};

std::string toString(ElementType type);

using ElementFunction = std::variant< // clang-format off
    GroupPtr,
    ReadablePtr,
    WritablePtr,
    ObservablePtr,
    CallablePtr
>; // clang-format on

/**
 * @brief Base Modeling element for any singular entity within the Device.
 *
 * Describes the modeled entity as well as provides the user with a
 * generic way of accessing the entity's functionality
 */
struct Element : public virtual MetaInfo {
  ~Element() override = default;

  virtual ElementType type() const = 0;

  virtual ElementFunction function() const = 0;

  friend bool operator==(const ElementPtr& lhs, const ElementPtr& rhs);

  friend bool operator!=(const ElementPtr& lhs, const ElementPtr& rhs);
};

using ElementPtr = std::shared_ptr<Element>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_ELEMENT_HPP