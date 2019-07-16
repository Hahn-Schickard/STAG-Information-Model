#ifndef _I_DEVICE_ELEMENT_GROUP_HPP
#define _I_DEVICE_ELEMENT_GROUP_HPP

#include "iDeviceElement.hpp"
#include "iNamedElement.hpp"
#include <string>
#include <vector>

namespace Information_Model {

class iDeviceElementGroup : public iDeviceElement {
public:
  virtual std::vector<iDeviceElement *> getSubelements() = 0;
  virtual iDeviceElement *getSubelement(const std::string refId) = 0;
};
}

#endif //_I_DEVICE_ELEMENT_GROUP_HPP