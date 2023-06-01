#ifndef __INFORMATION_MODEL_METRIC_MOCK_HPP_
#define __INFORMATION_MODEL_METRIC_MOCK_HPP_

#include "../Metric.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
namespace testing {
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

  MockMetric() : MockMetric(DataType::BOOLEAN) {}

  MockMetric(DataType type) : MockMetric(type, setVariant(type)) {}

  MockMetric(DataType type, const DataVariant& variant)
      : Metric(), type_(type), value_(variant) {}

  ~MockMetric() { ::testing::Mock::VerifyAndClear(this); }

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));

  void delegateToFake() {
    ON_CALL(*this, getMetricValue).WillByDefault(::testing::Return(value_));
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  void delegateToFake(Reader reader) {
    read_ = reader;
    ON_CALL(*this, getMetricValue).WillByDefault([this]() -> DataVariant {
      auto reader = read_.value();
      return reader();
    });
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }

protected:
  DataType type_;
  DataVariant value_;
  std::optional<Reader> read_ = std::nullopt;
};

using MockMetricPtr = std::shared_ptr<MockMetric>;

/** @}*/
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_MOCK_HPP_