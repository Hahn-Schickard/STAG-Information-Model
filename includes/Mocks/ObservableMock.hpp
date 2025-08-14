#ifndef __STAG_INFORMATION_MODEL_OBSERVABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_OBSERVABLE_MOCK_HPP
#include "Observable.hpp"

#include "ReadableMock.hpp"

#include <gmock/gmock.h>
#include <vector>

namespace Information_Model::testing {

struct ObservableMock : virtual public Observable, public ReadableMock {
  using IsObservingCallback = std::function<void(bool)>;

  ObservableMock() = default;

  explicit ObservableMock(DataType type) : ReadableMock(type) {}

  explicit ObservableMock(const DataVariant& value) : ReadableMock(value) {}

  ObservableMock(DataType type, const ReadCallback& read_cb)
      : ReadableMock(type, read_cb) {}

  ~ObservableMock() override = default;

  void enableSubscribeFaking(const IsObservingCallback& callback) {
    if (callback) {
      is_observing_ = callback;
      ON_CALL(*this, subscribe)
          .WillByDefault(
              ::testing::Invoke(this, &ObservableMock::attachObserver));
      ON_CALL(*this, notify)
          .WillByDefault(
              ::testing::Invoke(this, &ObservableMock::notifyObservers));
    }
  }

  MOCK_METHOD(void, notify, (const DataVariant&), (const final));
  MOCK_METHOD(ObserverPtr,
      subscribe,
      (const Observable::ObserveCallback&, const Observable::ExceptionHandler&),
      (final));

private:
  struct ObserverImpl : public Observer {
    explicit ObserverImpl(const Observable::ObserveCallback& callback,
        const Observable::ExceptionHandler& handler)
        : callback_(callback), handler_(handler) {}

    void notify(const std::shared_ptr<DataVariant>& value) {
      try {
        callback_(value);
      } catch (...) {
        handler_(std::current_exception());
      }
    }

  private:
    Observable::ObserveCallback callback_;
    Observable::ExceptionHandler handler_;
  };

  ObserverPtr attachObserver(const Observable::ObserveCallback& callback,
      const Observable::ExceptionHandler& handler) {
    auto observer = std::make_shared<ObserverImpl>(callback, handler);
    if (observers_.empty()) {
      is_observing_(true);
    }
    observers_.emplace_back(observer);
    return observer;
  }

  void notifyObservers(const DataVariant& value) {
    auto value_ptr = std::make_shared<DataVariant>(value);
    for (auto it = observers_.begin(); it != observers_.end();) {
      if (auto observer = it->lock()) {
        observer->notify(value_ptr);
        ++it;
      } else {
        it = observers_.erase(it);
      }
    }
    if (observers_.empty()) {
      is_observing_(false);
    }
  }

  IsObservingCallback is_observing_;
  std::vector<std::weak_ptr<ObserverImpl>> observers_;
};

using ObservableMockPtr = std::shared_ptr<ObservableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_OBSERVABLE_MOCK_HPP