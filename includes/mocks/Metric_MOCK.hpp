#ifndef __INFORMATION_MODEL_METRIC_MOCK_HPP_
#define __INFORMATION_MODEL_METRIC_MOCK_HPP_

#include "../Metric.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
namespace testing {
/**
 * @brief Metric mock implementation. Use only for testing!
 *
 */
class MockMetric : public Metric {
  DataType type_;
  DataVariant value_;

public:
  MockMetric()
      : Metric(), type_(DataType::UNKNOWN),
        value_(DataVariant((bool)false)) {}

  MockMetric(DataType type)
      : Metric(), type_(type), value_(setVariant(type_)) {}

  MockMetric(DataType type, const DataVariant &variant)
      : Metric(), type_(type), value_(variant) {}

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));

  void delegateToFake() {
    ON_CALL(*this, getMetricValue).WillByDefault([this]() -> DataVariant {
      return value_;
    });
    ON_CALL(*this, getDataType).WillByDefault([this]() -> DataType {
      return type_;
    });
  }

  void delegateToFake(std::function<DataVariant()> callback) {
    ON_CALL(*this, getMetricValue).WillByDefault([callback]() -> DataVariant {
      return callback();
    });
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }

  virtual ~MockMetric() {}
};

using MockMetricPtr = std::shared_ptr<MockMetric>;
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_MOCK_HPP_