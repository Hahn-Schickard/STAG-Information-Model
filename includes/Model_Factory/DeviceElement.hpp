#ifndef DEVICEELEMENT_H
#define DEVICEELEMENT_H

#include "NamedElement.hpp"
#include "iDeviceElement.hpp"

namespace Model_Factory {
using namespace Information_Model;

class DeviceElement : public iDeviceElement, public NamedElement {
private:
  ElementType elementType = Undefined;

public:
  DeviceElement(const std::string refId, const std::string name,
                const std::string desc, ElementType type);
  ElementType getElementType();
};
}

#endif // !DEVICEELEMENT_H