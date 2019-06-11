#ifndef DEVICEELEMENTGROUP_H
#define DEVICEELEMENTGROUP_H


#include "deviceElement.hpp"
#include <algorithm>

class DeviceElementGroup : public DeviceElement
{
protected:
	/*string name;
	string description;
	string refId;*/

	vector<DeviceElement*> subElements;

	
public:
	DeviceElementGroup(string refId, string name, string desc);
	vector<DeviceElement*> getSubElements();
	void addElement(DeviceElement &element);
	DeviceElement* GetElementByRefId(string refId)
	{
		auto elem = std::find_if(subElements.begin(), subElements.end(), [refId](DeviceElement* obj) { return obj->getReferenceId() == refId; });
		if (elem != subElements.end())
		{
			return *elem;
		}
		return *elem;
	}


};

#endif // !DEVICEELEMENTGROUP_H


