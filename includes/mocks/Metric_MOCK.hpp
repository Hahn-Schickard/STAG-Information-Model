#ifndef __INFORMATION_MODEL_METRIC_MOCK_HPP_
#define __INFORMATION_MODEL_METRIC_MOCK_HPP_

#include "../Metric.hpp"

#include "gmock/gmock.h"

namespace Information_Model::testing {
/**
 * @addtogroup ReadableModeling Metric Modelling
 * @{
 */
/**
 * @brief Metric mock implementation with ability to easily convert into a Fake
 *
 * @attention
 * Use only for testing
 *
 */
struct MockMetric : public Metric {
  using Reader = std::function<DataVariant()>;

  MockMetric() : MockMetric(DataType::Boolean) {}

  explicit MockMetric(DataType type)
      : MockMetric(type, setVariant(type).value()) {}

  MockMetric(DataType type, const DataVariant& variant)
      : Metric(type), value_(variant) {
    delegateToFake();
  }

  ~MockMetric() { ::testing::Mock::VerifyAndClear(this); }

  MOCK_METHOD(DataVariant, getMetricValue, (), (const override));

  void delegateToFake() {
    ON_CALL(*this, getMetricValue)
        .WillByDefault(std::bind(&MockMetric::returnValue, this));
  }

  void delegateToFake(const Reader& reader) {
    read_ = reader;
    if (read_) {
      ON_CALL(*this, getMetricValue)
          .WillByDefault(std::bind(&MockMetric::readValue, this));
    } else {
      delegateToFake();
    }
  }

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }

private:
  DataVariant returnValue() const { return value_; }

  DataVariant readValue() const { return read_(); }

  DataVariant value_;
  Reader read_ = nullptr;
};

using MockMetricPtr = std::shared_ptr<MockMetric>;
using NonemptyMockMetricPtr = Nonempty::Pointer<MockMetricPtr>;
/** @}*/
} // namespace Information_Model::testing

#endif //__INFORMATION_MODEL_METRIC_MOCK_HPP_