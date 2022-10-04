#ifndef __INFORMATION_MODEL_DEVICE_ELEMENT_HPP
#define __INFORMATION_MODEL_DEVICE_ELEMENT_HPP

#include "DeviceElementGroup.hpp"
#include "Metric.hpp"
#include "NamedElement.hpp"
#include "WritableMetric.hpp"

#include <memory>
#include <string>

namespace Information_Model {

struct DeviceElement : NamedElement {
  using SpecificInterface = std::variant<NonemptyDeviceElementGroupPtr,
      NonemptyMetricPtr, NonemptyWritableMetricPtr>;

  DeviceElement() = default;
  DeviceElement(const std::string& ref_id, const std::string& name,
      const std::string& desc, SpecificInterface&& interface)
      : NamedElement(ref_id, name, desc),
        specific_interface(std::move(interface)) {}

  const SpecificInterface specific_interface;
};

using DeviceElementPtr = std::shared_ptr<DeviceElement>;
using NonemptyDeviceElementPtr = NonemptyPointer::NonemptyPtr<DeviceElementPtr>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_ELEMENT_HPP