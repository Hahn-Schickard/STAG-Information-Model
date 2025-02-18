#include "DeviceMockBuilder.hpp"

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <thread>

using namespace std;
using namespace Information_Model;

void print(const DevicePtr& device);
void print(const NonemptyDeviceElementPtr& element, size_t offset);
void print(const NonemptyWritableMetricPtr& element, size_t offset);
void print(const NonemptyMetricPtr& element, size_t offset);
void print(const NonemptyObservableMetricPtr& element, size_t offset);
void print(const NonemptyFunctionPtr& element, size_t offset);
void print(const NonemptyDeviceElementGroupPtr& elements, size_t offset);

struct Executor {
  using Callback = function<void(void)>;
  Executor() : callback_([]() { cout << "Callback called" << endl; }) {}

  pair<uintmax_t, future<DataVariant>> execute(const Function::Parameters&) {
    auto execute_lock = lock_guard(execute_mx_);
    auto id = calls_.size();
    auto promise = std::promise<DataVariant>();
    auto future = make_pair(id, promise.get_future());
    calls_.emplace(id, move(promise));
    return future;
  }

  void cancel(uintmax_t id) {
    auto iter = calls_.find(id);
    if (iter != calls_.end()) {
      iter->second.set_exception(
          make_exception_ptr(CallCanceled(id, "ExternalExecutor")));
      auto clear_lock = lock_guard(erase_mx_);
      iter = calls_.erase(iter);
    } else {
      throw CallerNotFound(id, "ExternalExecutor");
    }
  }

  void respondAll() {
    auto execute_lock = lock_guard(execute_mx_);
    auto erase_lock = lock_guard(erase_mx_);
    // NOLINTNEXTLINE(modernize-loop-convert)
    for (auto it = calls_.begin(); it != calls_.end(); it++) {
      callback_();
      it->second.set_value(DataVariant());
    }
    calls_.clear();
  }

private:
  void respond(uintmax_t id) {
    auto erase_lock = lock_guard(erase_mx_);
    auto it = calls_.find(id);
    if (it != calls_.end()) {
      if (callback_) {
        callback_();
        it->second.set_value(DataVariant());
      } else {
        it->second.set_exception(
            make_exception_ptr(runtime_error("Callback dos not exist")));
      }
      it = calls_.erase(it);
    }
  }

  Callback callback_;
  mutex execute_mx_;
  mutex erase_mx_;
  unordered_map<uintmax_t, promise<DataVariant>> calls_;
};
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
    auto executor = make_shared<Executor>();
    DevicePtr device;
    string readable_id;
    string callable_id;
    string executable_id;
    string custom_executable_id;
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
      readable_id = integer_ref_id;
      builder->addWritableMetric(
          "WritesString", "Mocked writable metric", DataType::STRING);
      observed_value = builder
                           ->addObservableMetric("ObservesFalse",
                               "Mocked observable metric",
                               DataType::BOOLEAN,
                               bind(&isObserved, placeholders::_1))
                           .second;
      callable_id = builder->addFunction(
          "ReturnsBoolean", "Mocked function with return", DataType::BOOLEAN);
      executable_id = builder->addFunction(
          "ReturnsNone", "Mocked function with no return", DataType::NONE);
      custom_executable_id = builder->addFunction("CustomExecutable",
          "Mocked function with custom executor that does not return any "
          "values",
          DataType::NONE,
          bind(&Executor::execute, executor, placeholders::_1),
          bind(&Executor::cancel, executor, placeholders::_1));

      device = move(builder->getResult());
      delete builder;
    }

    print(device);

    auto readable = get<NonemptyMetricPtr>(
        device->getDeviceElement(readable_id)->functionality);
    auto value = get<intmax_t>(readable->getMetricValue());
    cout << "Reading " << readable_id << " metric value as " << value << endl;

    auto callable = get<NonemptyFunctionPtr>(
        device->getDeviceElement(callable_id)->functionality);
    auto call_result = callable->call();
    match(
        call_result,
        [callable_id](bool result) {
          cout << "Function " + callable_id + " result: "
               << (result ? "true" : "false") << endl;
        },
        [callable_id](auto) {
          cerr << "Received wrong result type from " + callable_id + " function"
               << endl;
        });

    auto executable = get<NonemptyFunctionPtr>(
        device->getDeviceElement(executable_id)->functionality);
    executable->execute();

    auto custom_executable = get<NonemptyFunctionPtr>(
        device->getDeviceElement(custom_executable_id)->functionality);
    custom_executable->execute();
    executor->respondAll();

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

  void handleEvent(DataVariantPtr value) override {
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
