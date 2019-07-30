#include "DeviceBuilder.hpp"
#include <iostream>
#include "informationModelExceptions.hpp"

using namespace std;
using namespace Model_Factory;
using namespace Information_Model;

string elementType_str(ElementType elementType)
{
  switch(elementType)
  {
    case ElementType::Undefined: return "Undefined";
    case ElementType::Group: return "Group";
     case ElementType::Readonly: return "Readonly";
     case ElementType::Observable: return "Observable";
     case ElementType::Writable: return "Writable";
     case ElementType::Function: return "Function";

  }
  return to_string(elementType);
}

void printElement(DeviceElement * element)
{
    cout << "Element: " << element->getElementName()
         << " has a ref_id: " << element->getElementRefId()
         << ", is of type: " << elementType_str(element->getElementType())
         << " and is described as: " << element->getElementDescription()
         << endl;
}

void printSubelements(vector<shared_ptr<DeviceElement>> elements)
{
  cout << "It has " << elements.size() << " elements" << endl;
  for (auto element : elements)
  {
    printElement(element.get());
    
  }

  for (auto element : elements)
  {
    if (element->getElementType() == Group)
    {
      cout << "\n\n==== SUBGROUP: " << element->getElementName() << " >>>\n" << endl;
      DeviceElementGroup* groupElement = static_cast<DeviceElementGroup*>(element.get());
      auto subElements = groupElement->getSubelements();
      if (subElements.size() > 0)
        printSubelements(subElements);
      cout << "\n<<< SUBGROUP: " << element->getElementName() << "\n" << endl;
    }
  }
}

void printDevice(Device *device)
{
  cout << "Device: " << device->getElementName()
       << " has a ref_id: " << device->getElementRefId()
       << " and is described as: " << device->getElementDescription() << endl;


  vector<shared_ptr<DeviceElement>> elements = device->getDeviceElementGroup()->getSubelements();
  printSubelements(elements);
  
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

  auto groupRefId = simple_device_builder->addDeviceElement("Device Subgroup1","A simple Group", ElementType::Group);
  simple_device_builder->addDeviceElement(groupRefId, "Observable Metric", "A simple observable Metric", ElementType::Observable);
  simple_device_builder->addDeviceElement(groupRefId, "Writable Metric", "A simple writeable metric", ElementType::Writable);
    groupRefId = simple_device_builder->addDeviceElement("Device Subgroup2","A simple Group", ElementType::Group);
   simple_device_builder->addDeviceElement(groupRefId, "Readonly Metric", "A simple Readonly Metric", ElementType::Readonly);
  simple_device_builder->addDeviceElement(groupRefId, "Function Metric", "A simple Function metric", ElementType::Function);
    groupRefId = simple_device_builder->addDeviceElement(groupRefId, "Level 2 Subgroup","A simple level 2 Subgrsoup", ElementType::Group);
   simple_device_builder->addDeviceElement(groupRefId, "Readonly Metric2", "A simple Readonly Metric", ElementType::Readonly);
  simple_device_builder->addDeviceElement(groupRefId, "Function Metric2", "A simple Function metric", ElementType::Function);
   groupRefId = simple_device_builder->addDeviceElement(groupRefId, "Level 3 Subgroup","A simple level 2 Subgroup", ElementType::Group);
   simple_device_builder->addDeviceElement(groupRefId, "Readonly Metric3", "A simple Readonly Metric", ElementType::Readonly);
  simple_device_builder->addDeviceElement(groupRefId, "Function Metric3", "A simple Function metric", ElementType::Function);
  

  unique_ptr<Device> local_scope_device = simple_device_builder->getDevice();

  delete simple_device_builder;

  cout << "\n\n>>>>> LOCAL SCOPE <<<<<<" << endl;
  printDevice(local_scope_device.get());

  unique_ptr<Device> externally_build_device = buildDeviceFromAFunction();

  cout << "\n\n>>>>>  EXTERNALLY BUILD  <<<<<<" << endl;
  printDevice(externally_build_device.get());

  cout << "\n\n>>>>>> EXCEPTION HANDLING <<<<<" << endl;

  try {
    DeviceBuilder *device_builder = new DeviceBuilder("Simple Device", "1234", "A simple device");
    device_builder->addDeviceElement(groupRefId, "Observable Metric", "A simple observable Metric", ElementType::Observable);
  }
  catch (GroupElementDoesNotExistException exception)
  {
      cout << "adding a metric caused a GroupElementDoesNotExistException: " << exception.what() << endl;
  }
  catch (exception ex)
  {
      cout << "adding a metric caused an exception: " << ex.what() << endl;
  }


  exit(0);
}