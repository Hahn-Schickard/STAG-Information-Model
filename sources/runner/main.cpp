#include "DeviceMockBuilder.hpp"

#include <atomic>
#include <condition_variable>
#include <iostream>

using namespace std;
using namespace Information_Model;

void print(const DevicePtr& device);
void print(const NonemptyDeviceElementPtr& element, size_t offset);
void print(const NonemptyWritableMetricPtr& element, size_t offset);
void print(const NonemptyMetricPtr& element, size_t offset);
void print(const NonemptyObservableMetricPtr& element, size_t offset);
void print(const NonemptyFunctionPtr& element, size_t offset);
void print(const NonemptyDeviceElementGroupPtr& elements, size_t offset);

DeviceBuilderInterface::ObservedValue observed_value = nullptr;

void isObserved(bool observed) {
  if (observed) {
    cout << "Starting observation" << endl;
    if (observed_value) {
      // delaying event dispatch so observer has time to initialize
      thread([] {
        cout << "Dispatching event" << endl;
        this_thread::sleep_for(100ms);
        observed_value(false);
      }).detach();
    } else {
      cerr << "ObservedValue callback is not set" << endl;
    }
  } else {
    cout << "Stopping observation" << endl;
  }
}

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
          "ReadsBoolean",
          "Mocked readable metric",
          DataType::BOOLEAN);
      auto integer_ref_id = builder->addReadableMetric(
          "ReadsInteger", "Mocked readable metric", DataType::INTEGER);
      read_target_id = integer_ref_id;
      builder->addWritableMetric(
          "WritesString", "Mocked writable metric", DataType::STRING);
      observed_value = builder
                           ->addObservableMetric("ObservesFalse",
                               "Mocked observable metric",
                               DataType::BOOLEAN,
                               bind(&isObserved, placeholders::_1))
                           .second;
      builder->addFunction(
          "ReturnsBoolean", "Mocked function with return", DataType::BOOLEAN);
      builder->addFunction(
          "ReturnsNone", "Mocked function with no return", DataType::NONE);

      device = move(builder->getResult());
      delete builder;
    }

    print(device);

    auto read_target_metric = get<NonemptyMetricPtr>(
        device->getDeviceElement(read_target_id)->functionality);
    auto value = get<intmax_t>(read_target_metric->getMetricValue());

    cout << "Reading " << read_target_id << " metric value as " << value
         << endl;

    observed_value = nullptr; // cleanup mocked callback
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

struct ExampleObserver : public MetricObserver {
  ExampleObserver(const NonemptyObservableMetricPtr& source)
      : MetricObserver(source) {}

  void handleEvent(shared_ptr<DataVariant> value) {
    {
      lock_guard lck(mx_);
      cout << "New value observed:  " << toString(*value) << endl;
      ready_ = true;
    }
    cv_.notify_one();
  }

  void waitForEvent() {
    unique_lock lck(mx_);
    cv_.wait(lck, [&] { return ready_.load(); });
  }

private:
  mutex mx_;
  condition_variable cv_;
  atomic<bool> ready_ = false;
};

void print(const NonemptyObservableMetricPtr& element, size_t offset) {
  cout << string(offset, ' ') << "Observes " << toString(element->getDataType())
       << " value: " << toString(element->getMetricValue()) << endl;
  auto observer = ExampleObserver(element);
  observer.waitForEvent();
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
      [offset](const NonemptyObservableMetricPtr& interface) { 
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
