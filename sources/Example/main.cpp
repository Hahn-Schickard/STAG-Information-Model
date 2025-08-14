#include "ObservableMock.hpp"
#include "WritableMock.hpp"

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

struct Stoppable {
  using Iteration = std::function<void()>;

  explicit Stoppable(const Iteration& iteration)
      : exit_future_(exit_signal_.get_future()), iteration_(iteration) {}

  virtual ~Stoppable() = default;

  void start() {
    do {
      iteration_();
    } while (!stopRequested());
  }

  bool stopRequested() {
    return !(exit_future_.wait_for(0ms) == std::future_status::timeout);
  }

  void stop() { exit_signal_.set_value(); }

private:
  std::promise<void> exit_signal_;
  std::future<void> exit_future_;
  Iteration iteration_;
};

struct Task {
  using ExceptionHandler = std::function<void(const std::exception_ptr&)>;

  Task(const Stoppable::Iteration& iteration, const ExceptionHandler& handler)
      : task_(std::make_unique<Stoppable>(iteration)), handler_(handler) {}

  virtual ~Task() {
    try {
      stop();
    } catch (...) {
      handler_(std::current_exception());
    }
  }

  void start() {
    if (!task_thread_) {
      task_thread_ = std::make_unique<std::thread>([this]() {
        try {
          task_->start();
        } catch (...) {
          handler_(std::current_exception());
        }
      });
    }
  }

  void stop() {
    if (task_thread_) {
      if (task_thread_->joinable()) {
        task_->stop();
        task_thread_->join();
      }
    }
  }

private:
  std::unique_ptr<Stoppable> task_;
  ExceptionHandler handler_;
  std::unique_ptr<std::thread> task_thread_;
};

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
  auto task = Task(
      [&notification_counter, observable_ptr = weak_ptr(observable)]() {
        if (auto observable = observable_ptr.lock()) {
          auto value = "Event " + to_string(notification_counter);
          observable->notify(value);
          ++notification_counter;
          this_thread::sleep_for(10ms);
        }
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
    if (observing) {
      std::unique_lock guard(mx);
      cout << "Starting to dispatch notifications" << endl;
      task.start();
    } else {
      std::unique_lock guard(mx);
      cout << "Stopping notification dispatch" << endl;
      task.stop();
    }
  });

  auto observer = observable->subscribe(
      [&mx](const shared_ptr<DataVariant>& updated) {
        std::unique_lock guard(mx);
        cout << "Received value update: " << toString(*updated) << endl;
      },
      [&mx](const std::exception_ptr& ex_ptr) {
        try {
          if (ex_ptr) {
            rethrow_exception(ex_ptr);
          }
        } catch (const std::exception& ex) {
          std::unique_lock guard(mx);
          cerr << "Observer caught an exception: " << ex.what() << endl;
        }
      });

  this_thread::sleep_for(100ms);
  observable->notify("Goodbye Everyone");

  observer.reset();

  observable->notify("No one is listening");

  return 0;
}