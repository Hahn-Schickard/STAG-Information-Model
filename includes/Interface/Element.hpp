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
  Group, /*!< Grouping element, aka list */
  Readable, /*!< Metric with read access */
  Writable, /*!< Metric with write access */
  Observable, /*!< Metric with read access and ability to self report
                 changes */
  Callable /*!< Metric with execute access */
};

inline std::string toString(ElementType type) {
  switch (type) {
  case ElementType::Group: {
    return "Group";
  }
  case ElementType::Readable: {
    return "Readable";
  }
  case ElementType::Writable: {
    return "Writable";
  }
  case ElementType::Observable: {
    return "Observable";
  }
  case ElementType::Callable: {
    return "Callable";
  }
  default: {
    throw std::logic_error("Could not decode ElementType enum value");
  }
  }
}

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
struct Element : public MetaInfo {
  ~Element() override = default;

  virtual ElementType type() const = 0;

  virtual ElementFunction function() const = 0;
};

using DeviceElementPtr = std::shared_ptr<DeviceElement>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_ELEMENT_HPP