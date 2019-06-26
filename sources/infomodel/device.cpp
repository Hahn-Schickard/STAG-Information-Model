#include "device.hpp"

using namespace std;

Device::Device(string refId, string name, string desc) : NamedElement(refId, name, desc)
{
	device_element_groups = vector<DeviceElementGroup*>();
}


void Device::addDeviceElementGroup(DeviceElementGroup * group) {
	this->device_element_groups.push_back(group);
}

DeviceElementGroup * Device::getDeviceElementGroup() {

	auto g1 = this->device_element_groups[0];

	return g1;
};

DeviceElementGroup * Device::getDeviceElementGroup(string refId) {

	for (int i = 0; i < device_element_groups.size(); i++)
	{
		if (device_element_groups[i]->getReferenceId() == refId)
		{
			return device_element_groups[i];
			break;
		}
		else if (device_element_groups[i]->getElementType() == Group)
		{
			auto group = findSubElementGroup(device_element_groups[i],refId);
			if (group != nullptr)
				return group;			
		}
		
	};
	return nullptr;
};

DeviceElementGroup* Device::findSubElementGroup(DeviceElementGroup* deviceElementGroup, std::string refId)
{
	auto subElementGroups = deviceElementGroup->getSubElementGroups();
	for (int j = 0; j < subElementGroups.size(); j++)
	{
		if (subElementGroups[j]->getReferenceId() == refId)
		{
			return subElementGroups[j];
			break;
		}
		else if (subElementGroups[j]->getElementType() == Group)
		{
			auto group = findSubElementGroup(subElementGroups[j], refId);
			if (group != nullptr)
				return group;
		}
	}
	return nullptr;
}

DeviceElement* Device::findSubElement(DeviceElementGroup* deviceElementGroup, std::string refId)
{
	auto subElements = deviceElementGroup->getSubElements();
	for (int j = 0; j < subElements.size(); j++)
	{
		if (subElements[j]->getReferenceId() == refId)
		{
			return subElements[j];
			break;
		}
		else if (subElements[j]->getElementType() == Group)
		{
			auto group = findSubElement((DeviceElementGroup*) subElements[j], refId);
			if (group != nullptr)
				return group;
		}
	}
	return nullptr;
}


vector<DeviceElementGroup *> Device::getDeviceElementGroups() {
		return device_element_groups;
};