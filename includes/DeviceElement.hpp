#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_HPP

#include "NamedElement.hpp"

#include <memory>
#include <string>

namespace Information_Model {
/**
 * @enum ElementTypeEnum
 * @brief ElementType enumeration, specifying the available DeviceElement types
 *
 */
enum class ElementType {
  UNDEFINED,  /*!< Fallback type */
  GROUP,      /*!< Groupping element, aka list */
  READABLE,   /*!< Metric with read access */
  OBSERVABLE, /*!< Metric with observation access */
  WRITABLE,   /*!< Metric with write access */
  FUNCTION    /*!< Function elements */
};

inline std::string toString(ElementType type) {
  switch (type) {
  case ElementType::GROUP: {
    return "Group";
  }
  case ElementType::READABLE: {
    return "Readable";
  }
  case ElementType::WRITABLE: {
    return "Writable";
  }
  case ElementType::OBSERVABLE: {
    return "Observable";
  }
  case ElementType::FUNCTION: {
    return "Function";
  }
  case ElementType::UNDEFINED:
  default: { return "UNDEFINED"; }
  }
}

class DeviceElement : public NamedElement {
  ElementType type_ = ElementType::UNDEFINED;

public:
  DeviceElement() = default;
  DeviceElement(const std::string &ref_id, const std::string &name,
                const std::string &desc, ElementType type)
      : NamedElement(ref_id, name, desc), type_(type) {}

  ElementType getElementType() { return type_; }
  virtual ~DeviceElement() = default;
};

using DeviceElementPtr = std::shared_ptr<DeviceElement>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_HPP