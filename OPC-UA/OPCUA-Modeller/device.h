#ifndef DEVICE_H
#define DEVICE_H

#include <stdlib.h>
#include <vector>
#include <string>

#include "deviceElementGroup.h"
#include "namedElement.h"

using namespace std;

class Device : public NamedElement
{
private:
	vector<DeviceElementGroup> device_elements;
	
public:
	Device(string refId, string name, string desc);
	
	~Device();

	void addDeviceElementGroup(DeviceElementGroup group);

	DeviceElementGroup GetDeviceElementGroup();
	
};

#endif // !DEVICE_H


