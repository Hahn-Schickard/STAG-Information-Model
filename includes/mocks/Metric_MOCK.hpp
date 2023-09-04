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

  MockMetric(DataType type)
      : // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
        MockMetric(type, setVariant(type).value()) {}

  MockMetric(DataType type, const DataVariant& variant)
      : Metric(type), value_(variant) {}

  ~MockMetric() { ::testing::Mock::VerifyAndClear(this); }

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));

  void delegateToFake() {
    ON_CALL(*this, getMetricValue).WillByDefault(::testing::Return(value_));
  }

  void delegateToFake(Reader reader) {
    read_ = reader;
    ON_CALL(*this, getMetricValue).WillByDefault([this]() -> DataVariant {
      if (read_) {
        return read_();
      } else {
        return value_;
      }
    });
  }

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }

protected:
  DataVariant value_;
  Reader read_ = nullptr;
};

using MockMetricPtr = std::shared_ptr<MockMetric>;
using NonemptyMockMetricPtr = NonemptyPointer::NonemptyPtr<MockMetricPtr>;
/** @}*/
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_MOCK_HPP_