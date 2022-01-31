#ifndef __INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_
#define __INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_

#include "../WritableMetric.hpp"
#include "Metric_MOCK.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
namespace testing {
/**
 * @brief WritableMetric mock implementation. Use only for testing!
 *
 */
class MockWritableMetric : public WritableMetric {
  DataType type_;
  DataVariant value_;

public:
  MockWritableMetric()
      : WritableMetric(), type_(DataType::UNKNOWN),
        value_(DataVariant((bool)false)) {}

  MockWritableMetric(DataType type)
      : WritableMetric(), type_(type),
        value_(setVariant(type_)) {}

  MockWritableMetric(DataType type,
                     const DataVariant &variant)
      : WritableMetric(), type_(type), value_(variant) {}

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(void, setMetricValue, (DataVariant /* value */), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));

  void delegateToFake() {
    ON_CALL(*this, getMetricValue).WillByDefault(::testing::Return(value_));
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  void delegateToFake(std::function<DataVariant()> callback) {
    ON_CALL(*this, getMetricValue).WillByDefault(callback);
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  void delegateToFake(std::function<DataVariant()> reader,
                      std::function<void(DataVariant)> writer) {
    ON_CALL(*this, getMetricValue).WillByDefault(reader);
    ON_CALL(*this, setMetricValue)
        .WillByDefault([this, writer](DataVariant value) { writer(value); });
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  ~MockWritableMetric() { ::testing::Mock::VerifyAndClear(this); }
};

using MockWritableMetricPtr = std::shared_ptr<MockWritableMetric>;
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_