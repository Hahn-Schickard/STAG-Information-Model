#include "blueprint.hpp"
#include "informationModelExceptions.hpp"
using namespace std;

using namespace std;

Device * BluePrint::GetDevice(std::string refId) 
{
	return this->device;
}

Device * BluePrint::GetDevice()
{
	return this->device;
}

BluePrint::BluePrint(string refId, string name, string desc)
{
	this->device = new Device(refId, name, desc);
}


std::string BluePrint::AddDeviceGroup(string name, string desc) {
	
	std::string groupRefId = generateRefId(this->device->getReferenceId());
	DeviceElementGroup * group = new DeviceElementGroup(groupRefId, name, desc);
	this->device->addDeviceElementGroup(group);
	return groupRefId;
}


void BluePrint::AddSubElement(string groupRefId, string name, string desc, ElementType elementType)
{
	AddSubElement(groupRefId, name, desc, elementType, ValueType::Unknown);
}

void BluePrint::AddSubElement(string groupRefId, string name, string desc, ElementType elementType, ValueType::ValueDataType valueType) {
	auto parent = device->getDeviceElementGroup(groupRefId);
	if (parent == nullptr) {
		throw ElementTypeMismatchException("No group element found with reference Id " + groupRefId);
	}
	if (parent->getElementType() != Group)
	{
		throw ElementTypeMismatchException("AddSubElment requires parent to be of type Group");
	}
	auto gRefId = generateRefId(groupRefId);

	DeviceElement * element = nullptr;
	switch (elementType)
	{
	case Group:
		element = new DeviceElementGroup(gRefId, name, desc);
		break;
	case Observable:
		element = new ObservableMetric(gRefId, name, desc, valueType);
		break;
	case Readonly:
		element = new DeviceMetric(gRefId, name, desc, valueType);
		break;
	case Writable:
		element = new WriteableMetric(gRefId, name, desc, valueType);
		break;

	}
	if (element != nullptr)
		parent->addElement(*element);
}

std::string BluePrint::generateRefId(string parentRefId) {
	auto parent = device->getDeviceElementGroup(parentRefId);
	//if nullPtr then it's the device
	if (parent == nullptr)
	{
		auto groups = device->getDeviceElementGroups();
		int iDeviceGroupId = groups.size() + 1;
		return parentRefId + "." + std::to_string(iDeviceGroupId);
	}
		

	auto elements = parent->getSubElements();
	int iGroupId = elements.size() + 1;
	std::string refId = parentRefId + "." + std::to_string(iGroupId);
	return refId;
}

