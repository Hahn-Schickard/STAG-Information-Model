#ifndef _I_DEVICE_HPP
#define _I_DEVICE_HPP

#include "NamedElement.hpp"
#include "iDeviceElementGroup.hpp"

namespace Information_Model {

class iDevice : public NamedElement {
protected:
  iDevice(const std::string REF_ID, const std::string NAME,
          const std::string DESC)
      : NamedElement(REF_ID, NAME, DESC) {}

public:
  virtual iDeviceElementGroup *getDeviceElementGroup() = 0;
};
}

#endif //_I_DEVICE_HPP