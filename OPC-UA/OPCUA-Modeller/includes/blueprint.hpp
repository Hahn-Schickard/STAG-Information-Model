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
public:
	

	virtual ~BluePrint() {}
	BluePrint(std::string refId, std::string name, std::string desc);
	
	BluePrint() {
		this->device = new Device("1234", "TestDevice", "A hardcoded device for testing only");
		DeviceElementGroup * group = new DeviceElementGroup("1234.1", "TestElementGroup", "A hardcoded device element group");
		ObservableMetric * observableMetric = new ObservableMetric("1234.1.1", "TestObservableMetric", "A hardcoded observableMetric Element",ValueType::String);
		group->addElement(*observableMetric);
		WriteableMetric * writeableMetric = new WriteableMetric("1234.1.2", "TestWriteableMetric", "A hardcoded writeableMetric", ValueType::Integer);
		group->addElement(*writeableMetric);
		DeviceMetric * deviceMetric = new DeviceMetric("1234.1.3", "TestDeviceMetric", "A hardcoded deviceMetric", ValueType::Float);
		group->addElement(*deviceMetric);
		DeviceElementGroup * subgroup = new DeviceElementGroup("1234.1.4", "TestElementGroup2", "A hardcoded device element group");
		DeviceMetric * deviceMetric2 = new DeviceMetric("1234.1.4.1", "TestDeviceMetric2", "A hardcoded deviceMetric", ValueType::Float);
		subgroup->addElement(*deviceMetric2);
		group->addElement(*subgroup);
		this->device->addDeviceElementGroup(group);
		
	}

	Device * GetDevice(std::string refId) ;

	Device * GetDevice() ;
	
};

#endif // !BLUEPRINT_H
