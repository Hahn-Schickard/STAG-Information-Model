#ifndef __STAG_INFORMATION_MODEL_OBSERVABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_OBSERVABLE_MOCK_HPP
#include "Observable.hpp"
#include "ReadableMock.hpp"

#include <gmock/gmock.h>
#include <mutex>
#include <vector>

namespace Information_Model::testing {

struct ObserverPimpl : virtual public Observer {
  ~ObserverPimpl() override = default;

  virtual void dispatch(const std::shared_ptr<DataVariant>& value) = 0;
};

struct ObservableMock : public Observable {
  using ReadCallback = ReadableMock::ReadCallback;
  using IsObservingCallback = std::function<void(bool)>;

  ObservableMock() = default;

  explicit ObservableMock(DataType type);

  explicit ObservableMock(const DataVariant& value);

  ObservableMock(DataType type, const ReadCallback& read_cb);

  ~ObservableMock() override = default;

  void enableSubscribeFaking(const IsObservingCallback& callback);

  void updateType(DataType type);

  void updateValue(const DataVariant& value);

  void updateReadCallback(const ReadCallback& read_cb);

  MOCK_METHOD(DataType, dataType, (), (const final));
  MOCK_METHOD(DataVariant, read, (), (const final));
  MOCK_METHOD(void, notify, (const DataVariant&), (const final));
  MOCK_METHOD(ObserverPtr,
      subscribe,
      (const Observable::ObserveCallback&, const Observable::ExceptionHandler&),
      (final));

private:
  void setReadableCalls();

  ObserverPtr attachObserver(const Observable::ObserveCallback& callback,
      const Observable::ExceptionHandler& handler);

  void notifyObservers(const DataVariant& value);

  ReadableMock readable_;
  std::mutex mx_;
  IsObservingCallback is_observing_;
  std::vector<std::weak_ptr<ObserverPimpl>> observers_;
};

using ObservableMockPtr = std::shared_ptr<ObservableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_OBSERVABLE_MOCK_HPP