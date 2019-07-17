#ifndef _I_DEVICE_HPP
#define _I_DEVICE_HPP

#include "DeviceElementGroup.hpp"
#include "NamedElement.hpp"
#include <string>

namespace Information_Model {

class Device : public NamedElement {
protected:
  Device(const std::string REF_ID, const std::string NAME,
         const std::string DESC)
      : NamedElement(REF_ID, NAME, DESC) {}

public:
  virtual DeviceElementGroup *getDeviceElementGroup() = 0;
};
}

#endif //_I_DEVICE_HPP