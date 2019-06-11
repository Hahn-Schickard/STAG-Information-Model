#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <stdlib.h>
#include <string>
#include "device.hpp"
#include "deviceMetric.hpp"

class QString
{
public:
	std::string getString();
};

class Bar
{
public:
	std::string value;
	std::string GetValue();
};

class BluePrint
{
private:
	Device * device;
	std::string generateRefId(string parentRefId);
public:
	

	virtual ~BluePrint() {}
	BluePrint(std::string refId, std::string name, std::string desc);
	
	BluePrint() {
		this->device = new Device("1234", "TestDevice", "A hardcoded device for testing only");
		AddDeviceGroup("TestElementGroup", "A hardcoded device element group");
		auto groupId = device->getDeviceElementGroups()[0]->getReferenceId();

		AddSubElement(groupId, "TestObservableMetric", "A hardcoded observableMetric Element", ElementType::Observable, ValueType::ValueDataType::String);
		AddSubElement(groupId, "TestWriteableMetric", "A hardcoded writeableMetric", ElementType::Writable, ValueType::ValueDataType::Integer);
		AddSubElement(groupId, "TestDeviceMetric", "A hardcoded deviceMetric", ElementType::Readonly, ValueType::ValueDataType::Float);
		AddSubElement(groupId, "TestElementGroup2", "A hardcoded device element group", ElementType::Group);
		auto group = device->getDeviceElementGroup(groupId);
		auto subGroupId = group->getSubElementGroups()[0]->getReferenceId();
		AddSubElement(subGroupId, "TestDeviceMetric2", "A hardcoded deviceMetric", ElementType::Readonly, ValueType::ValueDataType::Float);
	}

	void AddDeviceGroup(string name, string desc);
	void AddElement(string name, string desc, ElementType elementType);
	void AddSubElement(string groupRefId, string name, string desc, ElementType elementType) ;
	void AddSubElement(string groupRefId, string name, string desc, ElementType elementType, ValueType::ValueDataType valueType);

	Device * GetDevice(std::string refId) ;

	Device * GetDevice() ;
	
};

#endif // !BLUEPRINT_H
