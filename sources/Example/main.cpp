#include "ObservableMock.hpp"
#include "WritableMock.hpp"

#include <Stoppable/Task.hpp>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;

int main() {
  DataVariant value = "Hello World";
  mutex mx;

  auto read_cb = [&value, &mx]() -> DataVariant {
    std::unique_lock guard(mx);
    cout << "Reading value: " << toString(value) << endl;
    return value;
  };

  auto write_cb = [&value, &mx](const DataVariant& new_value) {
    std::unique_lock guard(mx);
    value = new_value;
    cout << "Writing value: " << toString(value) << endl;
  };

  auto writable = make_shared<::testing::NiceMock<WritableMock>>(
      toDataType(value), read_cb, write_cb);

  writable->read();
  writable->write("Hello Universe");
  writable->read();

  auto observable = make_shared<::testing::NiceMock<ObservableMock>>(
      toDataType(value), read_cb);

  size_t notification_counter = 0;
  auto task = Stoppable::Task(
      [&notification_counter, observable_ptr = weak_ptr(observable)]() {
        if (auto observable = observable_ptr.lock()) {
          auto value = "Event " + to_string(notification_counter);
          observable->notify(value);
          ++notification_counter;
        }
        this_thread::sleep_for(10ms);
      },
      [&mx](const std::exception_ptr& ex_ptr) {
        try {
          if (ex_ptr) {
            rethrow_exception(ex_ptr);
          }
        } catch (const std::exception& ex) {
          std::unique_lock guard(mx);
          cerr << "Stoppable task caught an exception: " << ex.what() << endl;
        }
      });
  observable->enableSubscribeFaking([&mx, &task](bool observing) {
    std::unique_lock guard(mx);
    if (observing) {
      cout << "Starting to dispatch notifications" << endl;
      task.start();
      cout << "Task has been started" << endl;
    } else {
      cout << "Stopping notification dispatch" << endl;
      task.stop();
      cout << "Task has been stopped" << endl;
    }
  });

  cout << "Attaching and observer" << endl;
  auto observer = observable->subscribe(
      [&mx](const shared_ptr<DataVariant>& updated) {
        std::unique_lock guard(mx);
        cout << "Received value update: " << toString(*updated) << endl;
      },
      [&mx](const std::exception_ptr& ex_ptr) {
        std::unique_lock guard(mx);
        try {
          if (ex_ptr) {
            rethrow_exception(ex_ptr);
          }
        } catch (const std::exception& ex) {
          cerr << "Observer caught an exception: " << ex.what() << endl;
        }
      });

  this_thread::sleep_for(100ms);
  observable->notify("Goodbye Everyone");

  observer.reset();
  cout << "Observer removed" << endl;

  observable->notify("No one is listening");
  cout << "No one received the last notification" << endl;
  cout << "Task is " << (task.running() ? "running" : "no longer running")
       << endl;

  return 0;
}