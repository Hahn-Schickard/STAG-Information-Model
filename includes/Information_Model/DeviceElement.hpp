#ifndef _I_DEVICE_ELEMENT_HPP
#define _I_DEVICE_ELEMENT_HPP

#include "NamedElement.hpp"
#include <string>

namespace Information_Model {

enum ElementType { Undefined, Group, Readonly, Observable, Writable, Function };

class DeviceElement : public NamedElement {
protected:
  ElementType elementType = ElementType::Undefined;

protected:
  DeviceElement(const std::string REF_ID, const std::string NAME,
                const std::string DESC, ElementType type)
      : NamedElement(REF_ID, NAME, DESC), elementType(type) {}

public:
  ElementType getElementType();
};
}
#endif //_I_DEVICE_ELEMENT_HPP