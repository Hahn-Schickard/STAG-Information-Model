#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <stdlib.h>
#include <string>
#include "device.hpp"
#include "deviceMetric.hpp"

using namespace std;

class BluePrint
{
public:
	string Name;
	string Desc;
	string RefId;

	Device * device;

	BluePrint() {
		this->device = new Device("1234", "TestDevice", "A hardcoded device for testing only");
		DeviceElementGroup * group = new DeviceElementGroup("1234.1", "TestElementGroup", "A hardcoded device element group");
		ObservableMetric * observableMetric = new ObservableMetric("1234.1.1", "TestObservableMetric", "A hardcoded observableMetric Element",ValueType::String);
		group->addElement(*observableMetric);
		this->device->addDeviceElementGroup(group);
	}

	Device * GetDevice(string refId)
	{
		return this->device;
	}
};

#endif // !BLUEPRINT_H
