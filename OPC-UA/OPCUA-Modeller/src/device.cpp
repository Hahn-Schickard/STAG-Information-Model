#include "../includes/device.hpp"

Device::Device(string refId, string name, string desc) : NamedElement(refId, name, desc)
{
	//device_elements = new std::vector<DeviceElementGroup>();
}
Device::~Device() {};

void Device::addDeviceElementGroup(DeviceElementGroup * group) {
	this->device_elements.push_back(group);
}

DeviceElementGroup * Device::getDeviceElementGroup() {

	auto g1 = this->device_elements[0];

	return g1;
};

DeviceElementGroup * Device::getDeviceElementGroup(string refId) {

	for (int i = 0; i < device_elements.size(); i++)
	{
		if (device_elements[i]->getReferenceId() == refId)
		{
			return device_elements[i];
			break;
		}
			
		
	};
	return nullptr;

};

vector<DeviceElementGroup *> Device::getDeviceElementGroups() {
		return device_elements;
};