#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_HPP

#include "DeviceElementGroup.hpp"
#include "Metric.hpp"
#include "NamedElement.hpp"
#include "WritableMetric.hpp"

#include <memory>
#include <string>

namespace Information_Model {

/**
 * @enum ElementTypeEnum
 * @brief ElementType enumeration, specifying the available DeviceElement
 * types.
 *
 */
enum class ElementType {
  GROUP, /*!< Grouping element, aka list */
  READABLE, /*!< Metric with read access */
  WRITABLE, /*!< Metric with write access */
  OBSERVABLE, /*!< Metric with read access and ability to self report
                 changes */
  FUNCTION /*!< Metric with execute access */
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
  default: {
    throw std::logic_error("Could not decode ElementType enum value");
  }
  }
}

struct DeviceElement : NamedElement {
  using SpecificInterface = std::variant<NonemptyDeviceElementGroupPtr,
      NonemptyMetricPtr,
      NonemptyWritableMetricPtr>;

  DeviceElement() = default;
  DeviceElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc,
      SpecificInterface&& interface)
      : NamedElement(ref_id, name, desc),
        specific_interface(std::move(interface)) {}

  const SpecificInterface specific_interface;

  ElementType getElementType() {
    if (std::holds_alternative<NonemptyDeviceElementGroupPtr>(
            specific_interface)) {
      return ElementType::GROUP;
    } else if (std::holds_alternative<NonemptyMetricPtr>(specific_interface)) {
      return ElementType::READABLE;
    } else if (std::holds_alternative<NonemptyWritableMetricPtr>(
                   specific_interface)) {
      return ElementType::WRITABLE;
    } else {
      throw std::runtime_error("Could not resolve ElementType");
    }
  }
};

using DeviceElementPtr = std::shared_ptr<DeviceElement>;
using NonemptyDeviceElementPtr = NonemptyPointer::NonemptyPtr<DeviceElementPtr>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_HPP