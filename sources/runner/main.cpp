#include "DeviceBuilder.hpp"
#include <iostream>

using namespace std;
using namespace Model_Factory;
using namespace Information_Model;

void printDevice(Device *device)
{
  cout << "Device: " << device->getElementName()
       << " has a ref_id: " << device->getElementRefId()
       << " and is described as: " << device->getElementDescription() << endl;
  std::vector<std::shared_ptr<Information_Model::DeviceElement>> elements =
      device->getDeviceElementGroup()->getSubelements();
  cout << "It has " << elements.size() << " elements" << endl;
  for (auto element : elements)
  {
    cout << "Element: " << element->getElementName()
         << " has a ref_id: " << element->getElementRefId()
         << ", is of type: " << element->getElementType()
         << " and is described as: " << element->getElementDescription()
         << endl;
  }
}

unique_ptr<Device> buildDeviceFromAFunction()
{
  DeviceBuilder *device_from_func_builder =
      new DeviceBuilder("Fun Device", "4321", "A device built in a function");
  device_from_func_builder->addDeviceElementGroup("Fun Group", "A fun group");
  device_from_func_builder->addDeviceElement(
      "Fun Element", "A fun readable Metric", ElementType::Readonly);

  unique_ptr<Device> tmp = device_from_func_builder->getDevice();

  delete device_from_func_builder;

  return move(tmp);
}

int main()
{
  DeviceBuilder *simple_device_builder =
      new DeviceBuilder("Simple Device", "1234", "A simple device");
  simple_device_builder->addDeviceElementGroup("Simple Group",
                                               "A simple group");
  simple_device_builder->addDeviceElement(
      "Simple Element", "A simple readable Metric", ElementType::Readonly);

  unique_ptr<Device> local_scope_device = simple_device_builder->getDevice();

  delete simple_device_builder;

  printDevice(local_scope_device.get());

  unique_ptr<Device> externally_build_device = buildDeviceFromAFunction();

  printDevice(externally_build_device.get());

  return 0;
}