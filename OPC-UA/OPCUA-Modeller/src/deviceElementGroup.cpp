#include "../includes/deviceElementGroup.hpp"

DeviceElementGroup::DeviceElementGroup(string refId, string name, string desc) : DeviceElement::DeviceElement(refId, name, desc) {};

vector<DeviceElement*> DeviceElementGroup::getSubElements() {
	return subElements;
}

void DeviceElementGroup::addElement(DeviceElement &element)
{
	subElements.push_back(&element);
}