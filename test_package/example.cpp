#include "Information_Model/mocks/DeviceMockBuilder.hpp"

#include "Information_Model/Function.hpp"
#include "Information_Model/Metric.hpp"
#include "Information_Model/ObservableMetric.hpp"
#include "Information_Model/WritableMetric.hpp"

#include <atomic>
#include <condition_variable>
#include <exception>
#include <iostream>
#include <vector>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;

int main() {
  try {
    DevicePtr device;
    vector<string> ids;
    {
      auto builder = make_shared<DeviceMockBuilder>();
      builder->buildDeviceBase("9876", "Mocky", "Mocked test device");
      ids.push_back(builder->addDeviceElementGroup("Group 1", "First group"));
      ids.push_back(builder->addReadableMetric(
          ids[0], "Status", "Mocked readable metric", DataType::BOOLEAN));
      ids.push_back(builder->addReadableMetric(
          "Temperature", "Mocked readable metric", DataType::INTEGER));
      ids.push_back(builder->addWritableMetric(
          "Label", "Mocked writable metric", DataType::STRING));
      ids.push_back(builder->addFunction(
          "Multiplicator", "Mocked function", DataType::DOUBLE));
      ids.push_back(builder->addFunction(
          "ReturnsBoolean", "Mocked function with return", DataType::BOOLEAN));
      ids.push_back(builder->addFunction(
          "ReturnsNone", "Mocked function with no return", DataType::NONE));
      ids.push_back(builder
                        ->addObservableMetric("ObservesFalse",
                            "Mocked observable metric",
                            DataType::BOOLEAN,
                            DeviceBuilderInterface::ObservedValue())
                        .first);

      device = move(builder->getResult());
    }

    for (const auto& id : ids) {
      auto element = device->getDeviceElement(id);
      match(
          element->functionality,
          [id](const NonemptyDeviceElementGroupPtr&) {
            cout << "Element " + id + " is a group" << endl;
          },
          [id](const NonemptyMetricPtr&) {
            cout << "Element " + id + " is a readable metric" << endl;
          },
          [id](const NonemptyWritableMetricPtr&) {
            cout << "Element " + id + " is a writable metric" << endl;
          },
          [id](const NonemptyObservableMetricPtr&) {
            cout << "Element " + id + " is an observable metric" << endl;
          },
          [id](const NonemptyFunctionPtr&) {
            cout << "Element " + id + " is a function" << endl;
          });
    }

    cout << "Integration test successful" << endl;
    return EXIT_SUCCESS;
  } catch (const exception& ex) {
    cerr << "An unhandled exception occurred during mock test. Exception: "
         << ex.what() << endl;
    return EXIT_FAILURE;
  }
}
