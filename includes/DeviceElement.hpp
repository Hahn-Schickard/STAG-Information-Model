#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_HPP

#include "DeviceElementGroup.hpp"
#include "Function.hpp"
#include "Metric.hpp"
#include "NamedElement.hpp"
#include "ObservableMetric.hpp"
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
  Group, /*!< Grouping element, aka list */
  Readable, /*!< Metric with read access */
  Writable, /*!< Metric with write access */
  Observable, /*!< Metric with read access and ability to self report
                 changes */
  Executable /*!< Metric with execute access */
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
  case ElementType::Executable: {
    return "Executable";
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
      NonemptyObservableMetricPtr,
      NonemptyFunctionPtr>; // clang-format on

  /**
   * @brief Provides a generic way of accessing the modeled entity's
   * functionality
   *
   * This variant can be used as an argument for Variant_Visitor:match()
   * helper function
   */
  // NOLINTNEXTLINE(readability-identifier-naming)
  SpecificInterface functionality;

  ElementType getElementType() const {
    if (std::holds_alternative<NonemptyDeviceElementGroupPtr>(functionality)) {
      return ElementType::Group;
    } else if (std::holds_alternative<NonemptyMetricPtr>(functionality)) {
      return ElementType::Readable;
    } else if (std::holds_alternative<NonemptyWritableMetricPtr>(
                   functionality)) {
      return ElementType::Writable;
    } else if (std::holds_alternative<NonemptyObservableMetricPtr>(
                   functionality)) {
      return ElementType::Observable;
    } else if (std::holds_alternative<NonemptyFunctionPtr>(functionality)) {
      return ElementType::Executable;
    } else {
      throw std::runtime_error("Could not resolve ElementType");
    }
  }

private:
  DeviceElement() = delete;
  DeviceElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc,
      SpecificInterface&& specific_interface)
      : NamedElement(ref_id, name, desc),
        functionality(std::move(specific_interface)) {}

  friend struct DeviceBuilderInterface;
};

inline bool operator==(
    const DeviceElement& lhs, const DeviceElement& rhs) noexcept {
  try {
    auto result = (const NamedElement&)lhs == ((const NamedElement&)rhs);
    result = result && (lhs.getElementType() == rhs.getElementType());
    if (result) {
      switch (lhs.getElementType()) {
      case ElementType::Writable: {
        result = result &&
            *(std::get<NonemptyWritableMetricPtr>(lhs.functionality)) ==
                *(std::get<NonemptyWritableMetricPtr>(rhs.functionality));
        break;
      }
      case ElementType::Readable: {
        result = result &&
            *(std::get<NonemptyMetricPtr>(lhs.functionality)) ==
                *(std::get<NonemptyMetricPtr>(rhs.functionality));
        break;
      }
      case ElementType::Executable: {
        result = result &&
            *(std::get<NonemptyFunctionPtr>(lhs.functionality)) ==
                *(std::get<NonemptyFunctionPtr>(rhs.functionality));
        break;
      }
      case ElementType::Observable: {
        result = result &&
            *(std::get<NonemptyObservableMetricPtr>(lhs.functionality)) ==
                *(std::get<NonemptyObservableMetricPtr>(rhs.functionality));
        break;
      }
      case ElementType::Group: {
        result = result &&
            *(std::get<NonemptyDeviceElementGroupPtr>(lhs.functionality)) ==
                *(std::get<NonemptyDeviceElementGroupPtr>(rhs.functionality));
        break;
      }
      default: {
        throw false;
      }
      }
    }
    return result;
  } catch (...) {
    return false;
  }
}

inline bool operator!=(
    const DeviceElement& lhs, const DeviceElement& rhs) noexcept {
  return !(lhs == rhs);
}

using DeviceElementPtr = std::shared_ptr<DeviceElement>;
using NonemptyDeviceElementPtr = Nonempty::Pointer<DeviceElementPtr>;
/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_HPP