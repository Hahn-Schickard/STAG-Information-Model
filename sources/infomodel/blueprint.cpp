#include "blueprint.hpp"
#include "informationModelExceptions.hpp"

using namespace std;

Device *Blueprint::GetDevice() { return device; }

Blueprint::Blueprint(string refId, string name, string desc) {
  this->device = new Device(refId, name, desc);
}

std::string Blueprint::AddDeviceGroup(string name, string desc) {

  std::string groupRefId =
      generateRefId(this->device->getReferenceId(), ElementType::Group);
  DeviceElementGroup *group = new DeviceElementGroup(groupRefId, name, desc);
  this->device->addDeviceElementGroup(group);
  return groupRefId;
}

std::string Blueprint::AddSubElement(string groupRefId, string name,
                                     string desc, ElementType elementType) {
  return AddSubElement(groupRefId, name, desc, elementType, ValueType::Unknown);
}

std::string Blueprint::AddSubElement(string groupRefId, string name,
                                     string desc, ElementType elementType,
                                     ValueType::ValueDataType valueType) {
  DeviceElementGroup *parent = device->getDeviceElementGroup(groupRefId);
  if (parent == nullptr) {
    throw ElementTypeMismatchException(
        "No group element found with reference Id " + groupRefId);
  }
  if (parent->getElementType() != Group) {
    throw ElementTypeMismatchException(
        "AddSubElment requires parent to be of type Group");
  }
  std::string gRefId = generateRefId(groupRefId, elementType);

  DeviceElement *element = nullptr;
  switch (elementType) {
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
  default:
    // TODO: Handle default behaviour
    break;
  }
  if (element != nullptr)
    parent->addElement(*element);
  return gRefId;
}

std::string Blueprint::generateRefId(string parentRefId,
                                     ElementType elementType) {
  std::string refId = "";
  DeviceElementGroup *parent = device->getDeviceElementGroup(parentRefId);
  // if nullPtr then it's the device
  if (parent == nullptr) {
    vector<DeviceElementGroup> groups = device->getDeviceElementGroups();
    int iDeviceGroupId = groups.size() + 1;
    refId = to_string(iDeviceGroupId);
  } else if (elementType != ElementType::Group) {
    vector<DeviceElement> elements = parent->getSubElements();
    size_t iGroupId = elements.size() + 1;
    refId = to_string(iGroupId);
  } else {
    // TODO: handle subgroups and extra DeviceElementGroups...
  }

  return parentRefId + "." + refId;
}
