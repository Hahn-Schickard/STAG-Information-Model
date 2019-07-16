#ifndef _I_DEVICE_HPP
#define _I_DEVICE_HPP

#include "iDeviceElementGroup.hpp"
#include "iNamedElement.hpp"

namespace Information_Model {

class iDevice : public iNamedElement {
public:
  virtual iDeviceElementGroup *getDeviceElementGroup() = 0;
};
}

#endif //_I_DEVICE_HPP