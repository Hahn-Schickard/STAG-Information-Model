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
 * @brief WritableMetric mock implementation with ability to easily convert into
 * a Fake
 *
 * @attention
 * Use only for testing
 *
 */
struct MockWritableMetric : public WritableMetric {
  using Writer = std::function<void(DataVariant)>;

  MockWritableMetric() : MockWritableMetric(DataType::BOOLEAN) {}

  MockWritableMetric(DataType type)
      // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
      : MockWritableMetric(type, setVariant(type).value()) {}

  MockWritableMetric(DataType type, const DataVariant& variant)
      : WritableMetric(type),
        readable_(
            std::make_shared<::testing::NiceMock<MockMetric>>(type, variant)) {
    ON_CALL(*this, getMetricValue)
        .WillByDefault(std::bind(&MockWritableMetric::readValue, this));
    delegateToFake();
  }

  MOCK_METHOD(void, setMetricValue, (DataVariant /* value */), (override));
  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(bool, isWriteOnly, (), (override));

  DataType getDataType() { return readable_->getDataType(); }

  void delegateToFake() { delegateToFake(MockMetric::Reader()); }

  void delegateToFake(MockMetric::Reader reader) {
    readable_->delegateToFake(reader);
    ON_CALL(*this, isWriteOnly).WillByDefault(::testing::Return(false));
  }

  void delegateToFake(Writer writer) {
    write_ = writer;
    ON_CALL(*this, getMetricValue)
        .WillByDefault(::testing::Throw(std::logic_error(
            "This metric does not support read functionality")));
    ON_CALL(*this, isWriteOnly).WillByDefault(::testing::Return(true));
  }

  void delegateToFake(Writer writer, MockMetric::Reader reader) {
    write_ = writer;
    if (write_) {
      ON_CALL(*this, setMetricValue)
          .WillByDefault(std::bind(
              &MockWritableMetric::writeValue, this, std::placeholders::_1));
    }
    delegateToFake(reader);
  }

  bool clearExpectations() {
    if (readable_->clearExpectations()) {
      return ::testing::Mock::VerifyAndClear(this);
    } else {
      return false;
    }
  }

private:
  void writeValue(DataVariant value) { write_(value); }

  DataVariant readValue() { return readable_->getMetricValue(); }

  Writer write_ = nullptr;
  std::shared_ptr<::testing::NiceMock<MockMetric>> readable_;
};

using MockWritableMetricPtr = std::shared_ptr<MockWritableMetric>;
using NonemptyMockWritableMetricPtr =
    NonemptyPointer::NonemptyPtr<MockWritableMetricPtr>;
/** @}*/
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_