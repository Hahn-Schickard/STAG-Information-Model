#ifndef __INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_
#define __INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_

#include "../WritableMetric.hpp"
#include "Metric_MOCK.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
namespace testing {
/**
 * @addtogroup WritableModeling Writable Metric Modelling
 * @{
 */
/**
 * @brief WritableMetric mock implementation. Use only for testing!
 *
 */
struct MockWritableMetric : public WritableMetric, public MockMetric {
  using Writer = std::function<void(DataVariant)>;
  // bring in base methods for overload resolution
  using MockMetric::delegateToFake;
  // resolve method ambiguity
  using MockMetric::getDataType;
  using MockMetric::getMetricValue;

  MockWritableMetric() : MockWritableMetric(DataType::BOOLEAN) {}

  MockWritableMetric(DataType type)
      : MockWritableMetric(type, setVariant(type)) {}

  MockWritableMetric(DataType type, const DataVariant& variant)
      : WritableMetric(), MockMetric(type, variant) {}

  MOCK_METHOD(void, setMetricValue, (DataVariant /* value */), (override));
  // redeclare metric mocks, so they are accessible
  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));
  MOCK_METHOD(bool, isWriteOnly, (), (override));

  void delegateToFake() {
    ON_CALL(*this, getMetricValue).WillByDefault(::testing::Return(value_));
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
    ON_CALL(*this, isWriteOnly).WillByDefault(::testing::Return(false));
  }

  void delegateToFake(Reader reader) {
    MockMetric::delegateToFake(reader);
    ON_CALL(*this, isWriteOnly).WillByDefault(::testing::Return(false));
  }

  void delegateToFake(Writer writer) {
    write_ = writer;
    ON_CALL(*this, getMetricValue)
        .WillByDefault(::testing::Throw(std::logic_error(
            "This metric does not support read functionality")));
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
    ON_CALL(*this, isWriteOnly).WillByDefault(::testing::Return(true));
  }

  void delegateToFake(Writer writer, Reader reader) {
    write_ = writer;
    ON_CALL(*this, setMetricValue).WillByDefault([this](DataVariant value) {
      if (write_) {
        write_(value);
      }
    });
    delegateToFake(reader);
    ON_CALL(*this, isWriteOnly).WillByDefault(::testing::Return(false));
  }

private:
  Writer write_ = nullptr;
};

using MockWritableMetricPtr = std::shared_ptr<MockWritableMetric>;

/** @}*/
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_