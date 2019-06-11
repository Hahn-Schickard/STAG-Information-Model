#ifndef DEVICE_H
#define DEVICE_H

#include <stdlib.h>
#include <vector>
#include <string>

#include "deviceElementGroup.hpp"
#include "namedElement.hpp"

using namespace std;

class Device : public NamedElement
{
private:
	vector<DeviceElementGroup*> device_elements;
	
public:
	Device(string refId, string name, string desc);
	


	void addDeviceElementGroup(DeviceElementGroup * group);

	DeviceElementGroup * getDeviceElementGroup();
	DeviceElementGroup * getDeviceElementGroup(string refId);
	vector<DeviceElementGroup*> getDeviceElementGroups();

	
};

#endif // !DEVICE_H


