#include "DeviceMockBuilder.hpp"

#include "Function.hpp"
#include "Metric.hpp"
#include "WritableMetric.hpp"

#include <iostream>

using namespace std;
using namespace Information_Model;

void print(const DevicePtr& device);
void print(const NonemptyDeviceElementPtr& element, size_t offset);
void print(const NonemptyWritableMetricPtr& element, size_t offset);
void print(const NonemptyMetricPtr& element, size_t offset);
void print(const NonemptyFunctionPtr& element, size_t offset);
void print(const NonemptyDeviceElementGroupPtr& elements, size_t offset);

int main() {
  try {
    DevicePtr device;
    string read_target_id;
    {
      auto* builder = new Information_Model::testing::DeviceMockBuilder();
      builder->buildDeviceBase("9876", "Mocky", "Mocked test device");
      auto subgroup_1_ref_id =
          builder->addDeviceElementGroup("Group 1", "First group");
      auto boolean_ref_id = builder->addReadableMetric(subgroup_1_ref_id,
          "Boolean",
          "Mocked readable metric",
          DataType::BOOLEAN);
      auto integer_ref_id = builder->addReadableMetric(
          "Integer", "Mocked readable metric", DataType::INTEGER);
      read_target_id = integer_ref_id;
      builder->addWritableMetric(
          "String", "Mocked writable metric", DataType::STRING);
      builder->addFunction("Boolean", "Mocked function", DataType::BOOLEAN);

      device = move(builder->getResult());
      delete builder;
    }

    print(device);

    auto read_target_metric = get<NonemptyMetricPtr>(
        device->getDeviceElement(read_target_id)->functionality);
    auto value = get<intmax_t>(read_target_metric->getMetricValue());

    cout << "Reading " << read_target_id << " metric value as " << value
         << endl;

    return EXIT_SUCCESS;
  } catch (const exception& ex) {
    cerr << "An unhandled exception occurred during mock test. Exception: "
         << ex.what() << endl;
    return EXIT_FAILURE;
  }
}

void print(const NonemptyDeviceElementGroupPtr& elements, size_t offset) {
  cout << string(offset, ' ') << "Group contains elements:" << endl;
  for (const auto& element : elements->getSubelements()) {
    print(element, offset + 3);
  }
}

void print(const NonemptyMetricPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << endl;
}

void print(const NonemptyWritableMetricPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Reads " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  cout << string(offset, ' ') << "Writes " << toString(element->getDataType())
       << " value type" << endl;
  cout << endl;
}

void print(const NonemptyFunctionPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Executes " << toString(element->result_type)
       << " call(" << toString(element->parameters) << ")" << endl;
  cout << endl;
}

void print(const NonemptyDeviceElementPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Element name: " << element->getElementName()
       << endl;
  cout << string(offset, ' ') << "Element id: " << element->getElementId()
       << endl;
  cout << string(offset, ' ')
       << "Described as: " << element->getElementDescription() << endl;

  match(
      element->functionality, // clang-format off
      [offset](const NonemptyDeviceElementGroupPtr& interface) {
        print(interface, offset);
      },
      [offset](const NonemptyMetricPtr& interface) {
         print(interface, offset); 
      },
      [offset](const NonemptyWritableMetricPtr& interface) { 
        print(interface, offset); 
      },
      [offset](const NonemptyFunctionPtr& interface) { 
        print(interface, offset); 
      }); // clang-format on
}

void print(const DevicePtr& device) {
  cout << "Device name: " << device->getElementName() << endl;
  cout << "Device id: " << device->getElementId() << endl;
  cout << "Described as: " << device->getElementDescription() << endl;
  cout << endl;
  print(device->getDeviceElementGroup(), 3);
}
