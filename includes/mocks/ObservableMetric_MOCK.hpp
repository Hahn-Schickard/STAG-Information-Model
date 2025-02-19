#ifndef __INFORMATION_MODEL_OBSERVABLE_METRIC_FAKE_HPP_
#define __INFORMATION_MODEL_OBSERVABLE_METRIC_FAKE_HPP_

#include "../ObservableMetric.hpp"
#include "Metric_MOCK.hpp"

#include "gmock/gmock.h"

namespace Information_Model::testing {
/**
 * @addtogroup ObservableModeling Observable Metric Modelling
 * @{
 */
/**
 * @brief Observable Metric mock implementation with ability to easily convert
 * into a Fake
 *
 * @attention
 * Use only for testing
 *
 */
struct MockObservableMetric : public ObservableMetric {
  using ObserveInitializer = std::function<void(bool)>;

  MockObservableMetric() : MockObservableMetric(DataType::BOOLEAN) {}

  explicit MockObservableMetric(const ObserveInitializer& ObserveInitializer)
      : MockObservableMetric(DataType::BOOLEAN, ObserveInitializer) {}

  explicit MockObservableMetric(DataType type)
      : MockObservableMetric(type, setVariant(type).value()) {}

  MockObservableMetric(
      DataType type, const ObserveInitializer& ObserveInitializer)
      : MockObservableMetric(
            type, setVariant(type).value(), ObserveInitializer) {}

  MockObservableMetric(DataType type, const DataVariant& variant)
      : MockObservableMetric(type,
            std::bind(&MockObservableMetric::handleException,
                this,
                std::placeholders::_1),
            variant,
            nullptr) {}

  MockObservableMetric(DataType type,
      const DataVariant& variant,
      const ObserveInitializer& ObserveInitializer)
      : MockObservableMetric(type,
            std::bind(&MockObservableMetric::handleException,
                this,
                std::placeholders::_1),
            variant,
            ObserveInitializer) {}

  MockObservableMetric(DataType type,
      const ObservableMetric::ExceptionHandler& handler,
      const DataVariant& variant,
      const ObserveInitializer& ObserveInitializer)
      : ObservableMetric(type, handler), readable_(type, variant),
        observe_(ObserveInitializer) {
    ON_CALL(*this, getMetricValue)
        .WillByDefault(std::bind(&MockObservableMetric::readValue, this));
    delegateToFake();
  }

  ~MockObservableMetric() { clearExpectations(); }

  MOCK_METHOD(DataVariant, getMetricValue, (), (const override));

  std::size_t attach(
      Event_Model::HandleEventCallback<DataVariant>&& listener_callback) final {
    bool has_listeners = hasListeners();
    auto listener_id = ObservableMetric::attach(std::move(listener_callback));
    if (!has_listeners) {
      if (observe_) {
        observe_(true);
      }
    }
    return listener_id;
  }

  void detach(std::size_t callback_id) final {
    ObservableMetric::detach(callback_id);
    if (!hasListeners()) {
      if (observe_) {
        observe_(false);
      }
    }
  }

  DataType getDataType() const { return readable_.getDataType(); }

  void delegateToFake() { delegateToFake(MockMetric::Reader()); }

  void delegateToFake(const MockMetric::Reader& reader) {
    readable_.delegateToFake(reader);
  }

  bool clearExpectations() {
    if (readable_.clearExpectations()) {
      return ::testing::Mock::VerifyAndClear(this);
    } else {
      return false;
    }
  }

private:
  void handleException(const std::exception_ptr&) const {
    /*surpress all exceptions*/
  }

  DataVariant readValue() const { return readable_.getMetricValue(); }

  ::testing::NiceMock<MockMetric> readable_;
  ObserveInitializer observe_;
};

using MockObservableMetricPtr = std::shared_ptr<MockObservableMetric>;
using NonemptyMockObservableMetricPtr =
    NonemptyPointer::NonemptyPtr<MockObservableMetricPtr>;

struct MockMetricObserver : public MetricObserver {
  explicit MockMetricObserver(const MockObservableMetricPtr& source)
      : MetricObserver(source) {}

  explicit MockMetricObserver(const NonemptyObservableMetricPtr& source)
      : MetricObserver(std::move(source)) {}

  ~MockMetricObserver() { clearExpectations(); }

  MOCK_METHOD(
      void, handleEvent, (std::shared_ptr<DataVariant> /*event*/), (override));

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }
};

using MockMetricObserverPtr = std::shared_ptr<MockMetricObserver>;
using NonemptyMockMetricObserverPtr =
    NonemptyPointer::NonemptyPtr<MockMetricObserverPtr>;
/** @}*/
} // namespace Information_Model::testing

#endif //__INFORMATION_MODEL_OBSERVABLE_METRIC_FAKE_HPP_