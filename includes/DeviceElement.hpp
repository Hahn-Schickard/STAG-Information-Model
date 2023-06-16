#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_HPP

#include "DeviceElementGroup.hpp"
#include "Function.hpp"
#include "Metric.hpp"
#include "NamedElement.hpp"
#include "WritableMetric.hpp"

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

/**
 * @brief Base Modeling element for any singular entity within the Device.
 *
 * Describes the modeled entity as well as provides the user with a
 * generic way of accessing the entity's functionality
 */
struct DeviceElement : public NamedElement {
  /**
   * @brief Contains modeled entity's functionality
   *
   * @see DeviceElementGroup
   * @see Metric
   * @see WritableMetric
   */
  using SpecificInterface = std::variant< // clang-format off
      NonemptyDeviceElementGroupPtr,
      NonemptyMetricPtr,
      NonemptyWritableMetricPtr,
      NonemptyFunctionPtr>; // clang-format on

  /**
   * @brief Provides a generic way of accessing the modeled entity's
   * functionality
   *
   * This variant can be used as an argument for Variant_Visitor:match()
   * helper function
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  const SpecificInterface functionality;

  ElementType getElementType() {
    if (std::holds_alternative<NonemptyDeviceElementGroupPtr>(functionality)) {
      return ElementType::GROUP;
    } else if (std::holds_alternative<NonemptyMetricPtr>(functionality)) {
      return ElementType::READABLE;
    } else if (std::holds_alternative<NonemptyWritableMetricPtr>(
                   functionality)) {
      return ElementType::WRITABLE;
    } else if (std::holds_alternative<NonemptyFunctionPtr>(functionality)) {
      return ElementType::FUNCTION;
    } else {
      throw std::runtime_error("Could not resolve ElementType");
    }
  }

private:
  DeviceElement() = delete;
  DeviceElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc,
      SpecificInterface&& interface)
      : NamedElement(ref_id, name, desc), functionality(std::move(interface)) {}

  friend struct DeviceBuilderInterface;
};

using DeviceElementPtr = std::shared_ptr<DeviceElement>;
using NonemptyDeviceElementPtr = NonemptyPointer::NonemptyPtr<DeviceElementPtr>;

/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_HPP