#include "ObservableMock.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

ObservableMock::ObservableMock(DataType type) : ReadableMock(type) {}

ObservableMock::ObservableMock(const DataVariant& value)
    : ReadableMock(value) {}

ObservableMock::ObservableMock(DataType type, const ReadCallback& read_cb)
    : ReadableMock(type, read_cb) {}

void ObservableMock::enableSubscribeFaking(
    const IsObservingCallback& callback) {
  if (callback) {
    is_observing_ = callback;
    ON_CALL(*this, subscribe)
        .WillByDefault(Invoke(this, &ObservableMock::attachObserver));
    ON_CALL(*this, notify)
        .WillByDefault(Invoke(this, &ObservableMock::notifyObservers));
  }
}

struct FakeObserver : public ObserverPimpl {
  explicit FakeObserver(const Observable::ObserveCallback& callback,
      const Observable::ExceptionHandler& handler)
      : callback_(callback), handler_(handler) {}

  ~FakeObserver() override = default;

  void dispatch(const shared_ptr<DataVariant>& value) override {
    unique_lock guard(mx_);
    try {
      callback_(value);
    } catch (...) {
      handler_(current_exception());
    }
  }

private:
  mutex mx_;
  Observable::ObserveCallback callback_;
  Observable::ExceptionHandler handler_;
};

ObserverPtr ObservableMock::attachObserver(
    const Observable::ObserveCallback& callback,
    const Observable::ExceptionHandler& handler) {
  unique_lock guard(mx_);
  auto was_empty = observers_.empty();
  auto observer = make_shared<FakeObserver>(callback, handler);
  observers_.emplace_back(observer);
  if (was_empty) {
    is_observing_(true);
  }
  return observer;
}

void ObservableMock::notifyObservers(const DataVariant& value) {
  unique_lock guard(mx_);
  { // we want value_ptr to run out of scope and be destroyed
    auto value_ptr = make_shared<DataVariant>(value);
    for (auto it = observers_.begin(); it != observers_.end();) {
      if (auto observer = it->lock()) {
        observer->dispatch(value_ptr);
        ++it;
      } else {
        it = observers_.erase(it);
      }
    }
  }
  if (observers_.empty()) {
    is_observing_(false);
  }
}
} // namespace Information_Model::testing
