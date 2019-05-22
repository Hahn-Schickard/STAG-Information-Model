#include "device.h"

Device::Device(string refId, string name, string desc) : NamedElement(refId, name, desc)
{

	//device_elements = new std::vector<DeviceElementGroup>();
}
Device::~Device() {};

void Device::addDeviceElementGroup(DeviceElementGroup group) {
	this->device_elements.push_back(group);
}

DeviceElementGroup Device::GetDeviceElementGroup() {
	DeviceElementGroup g1 = this->device_elements[0];

	return g1;
};