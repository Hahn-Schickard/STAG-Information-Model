#ifndef __DATA_MODEL_DEVICE_HPP
#define __DATA_MODEL_DEVICE_HPP

#include "DeviceElement.hpp"
#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"

#include <memory>
#include <string>

namespace Information_Model {
class Device : public NamedElement {
protected:
  Device(const std::string REF_ID, const std::string NAME,
         const std::string DESC)
      : NamedElement(REF_ID, NAME, DESC) {}

public:
  virtual std::shared_ptr<DeviceElementGroup> getDeviceElementGroup() = 0;
  virtual std::shared_ptr<DeviceElement>
  getDeviceElement(const std::string &ref_id) = 0;
};

} // namespace Information_Model

#endif //__DATA_MODEL_DEVICE_HPP