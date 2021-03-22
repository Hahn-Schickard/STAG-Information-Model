#include "Information_Model/mocks/DeviceMockBuilder.hpp"

#include "Information_Model/Metric.hpp"
#include "Information_Model/WritableMetric.hpp"

#include <iostream>

using namespace std;
using namespace Information_Model;

void print(DevicePtr device);
void print(DeviceElementPtr element, size_t offset);
void print(WritableMetricPtr element, size_t offset);
void print(MetricPtr element, size_t offset);
void print(DeviceElementGroupPtr elements, size_t offset);

int main() {
  DevicePtr device;
  {
    auto builder = new Information_Model::testing::DeviceMockBuilder();
    builder->buildDeviceBase("9876", "Mocky", "Mocked test device");
    auto subgroup_1_ref_id =
        builder->addDeviceElementGroup("Group 1", "First group");
    auto boolean_ref_id =
        builder->addReadableMetric(subgroup_1_ref_id, "Boolean",
                                   "Mocked readable metric", DataType::BOOLEAN);
    auto integer_ref_id = builder->addReadableMetric(
        "Integer", "Mocked readable metric", DataType::INTEGER);
    auto string_ref_id = builder->addReadableMetric(
        "String", "Mocked readable metric", DataType::STRING);

    device = builder->getResult();
    delete builder;
  }

  print(device);

  return EXIT_SUCCESS;
}

void print(DeviceElementGroupPtr elements, size_t offset) {
  cout << string(offset, ' ') << "Group contains elements:" << endl;
  for (auto element : elements->getSubelements()) {
    print(element, offset + 3);
  }
}

void print(MetricPtr element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << endl;
}

void print(WritableMetricPtr element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << string(offset, ' ') << "Writes " << toString(element->getDataType())
       << " value type" << endl;
  cout << endl;
}

void print(DeviceElementPtr element, size_t offset) {
  cout << string(offset, ' ') << "Element name: " << element->getElementName()
       << endl;
  cout << string(offset, ' ') << "Element id: " << element->getElementId()
       << endl;
  cout << string(offset, ' ')
       << "Described as: " << element->getElementDescription() << endl;
  cout << string(offset, ' ')
       << "Element type: " << toString(element->getElementType()) << endl;

  switch (element->getElementType()) {
  case ElementType::GROUP: {
    print(static_pointer_cast<DeviceElementGroup>(element), offset);
    break;
  }
  case ElementType::READABLE: {
    print(static_pointer_cast<Metric>(element), offset);
    break;
  }
  case ElementType::WRITABLE: {
    print(static_pointer_cast<WritableMetric>(element), offset);
    break;
  }
  case ElementType::FUNCTION: {
    cerr << string(offset, ' ') << "Function element types are not implemented!"
         << endl;
    break;
  }
  case ElementType::OBSERVABLE: {
    cerr << string(offset, ' ')
         << "Observable elements types are not implemented!" << endl;
    break;
  }
  case ElementType::UNDEFINED:
  default: {
    cerr << string(offset, ' ') << "Is not a valid element type!" << endl;
    break;
  }
  }
}

void print(DevicePtr device) {
  cout << "Device name: " << device->getElementName() << endl;
  cout << "Device id: " << device->getElementId() << endl;
  cout << "Described as: " << device->getElementDescription() << endl;
  cout << endl;
  print(device->getDeviceElementGroup(), 3);
}
