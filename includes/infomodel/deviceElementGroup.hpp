#ifndef DEVICEELEMENTGROUP_H
#define DEVICEELEMENTGROUP_H

#include "deviceElement.hpp"
#include <algorithm>
#include <string>
#include <vector>

class DeviceElementGroup : public DeviceElement {
protected:
  std::vector<DeviceElement> subElements;

public:
  DeviceElementGroup(std::string refId, std::string name, std::string desc);
  std::vector<DeviceElement> getSubElements();
  DeviceElement *GetElementByRefId(std::string refId);
  void addElement(DeviceElement &element);
};

#endif // !DEVICEELEMENTGROUP_H