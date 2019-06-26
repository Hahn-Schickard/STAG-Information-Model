#include "deviceElementGroup.hpp"

using namespace std;

DeviceElementGroup::DeviceElementGroup(string refId, string name, string desc) : DeviceElement::DeviceElement(refId, name, desc)
{
	this->elementType = ElementType::Group;
};

vector<DeviceElement*> DeviceElementGroup::getSubElements() {
	return subElements;
}

vector<DeviceElementGroup*> DeviceElementGroup::getSubElementGroups() {
	vector<DeviceElementGroup*> elementGroups = vector<DeviceElementGroup*>();
	for (int i = 0; i < subElements.size(); i++)
	{
		if (subElements[i]->getElementType() == Group)
		{
			elementGroups.push_back((DeviceElementGroup*)subElements[i]);
		}		
	}
	return elementGroups;
}

void DeviceElementGroup::addElement(DeviceElement &element)
{
	subElements.push_back(&element);
}