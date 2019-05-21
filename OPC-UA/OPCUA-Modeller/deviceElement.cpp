#include "deviceElement.h"

DeviceElement::DeviceElement(string refId, string name, string desc) : NamedElement(refId, name, desc) {

}

ElementType DeviceElement::getElementType()
{
	return this->elementType;
}