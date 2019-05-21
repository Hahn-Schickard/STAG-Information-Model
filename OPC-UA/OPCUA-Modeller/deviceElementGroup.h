#ifndef DEVICEELEMENTGROUP_H
#define DEVICEELEMENTGROUP_H


#include "deviceElement.h"

class DeviceElementGroup : public DeviceElement
{
protected:
	string name;
	string description;
	string refId;

	vector<DeviceElement*> subElements;

	
public:
	DeviceElementGroup(string refId, string name, string desc);
	vector<DeviceElement*> getSubElements();
	void addElement(DeviceElement &element);


};

#endif // !DEVICEELEMENTGROUP_H


