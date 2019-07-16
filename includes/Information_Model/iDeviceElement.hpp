#ifndef _I_DEVICE_ELEMENT_HPP
#define _I_DEVICE_ELEMENT_HPP

#include "iNamedElement.hpp"

namespace Information_Model {

enum ElementType { Undefined, Group, Readonly, Observable, Writable, Function };

class iDeviceElement : public iNamedElement {
public:
  virtual ElementType getElementType() = 0;
};
}
#endif //_I_DEVICE_ELEMENT_HPP