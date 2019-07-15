#ifndef DEVICEELEMENT_H
#define DEVICEELEMENT_H

#include "iDeviceElement.hpp"
#include "NamedElement.hpp"

class DeviceElement : public iDeviceElement, public NamedElement
{
public:
	DeviceElement(const std::string refId, const std::string name, const std::string desc, ElementType type);
	ElementType getElementType();

private:
	ElementType elementType = Undefined;
};

#endif // !DEVICEELEMENT_H