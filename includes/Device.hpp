#ifndef __INFORMATION_MODEL_DEVICE_HPP
#define __INFORMATION_MODEL_DEVICE_HPP

#include "DeviceElement.hpp"
#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"

#include <memory>
#include <string>

namespace Information_Model {
class Device : public NamedElement {
protected:
  Device(const std::string &ref_id, const std::string &name,
         const std::string &desc)
      : NamedElement(ref_id, name, desc) {}

public:
  virtual DeviceElementGroupPtr getDeviceElementGroup() = 0;
  virtual DeviceElementPtr getDeviceElement(const std::string &ref_id) = 0;

  virtual ~Device() = default;
};

using DevicePtr = std::shared_ptr<Device>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_HPP