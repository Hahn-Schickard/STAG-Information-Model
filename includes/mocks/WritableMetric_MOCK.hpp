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
  MockWritableMetric() = delete;

  MockWritableMetric(const std::string &ref_id, const std::string &name,
                     const std::string &desc)
      : WritableMetric(ref_id, name, desc), type_(DataType::UNKNOWN),
        value_(DataVariant((bool)false)) {}

  MockWritableMetric(const std::string &ref_id, const std::string &name,
                     const std::string &desc, DataType type)
      : WritableMetric(ref_id, name, desc), type_(type),
        value_(setVariant(type_)) {}

  MockWritableMetric(const std::string &ref_id, const std::string &name,
                     const std::string &desc, DataType type,
                     const DataVariant &variant)
      : WritableMetric(ref_id, name, desc), type_(type), value_(variant) {}

  MOCK_METHOD(std::size_t, size, (), (override));
  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(void, setMetricValue, (DataVariant /* value */), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));

  void delegateToFake() {
    ON_CALL(*this, size).WillByDefault(::testing::Return(size_of(value_)));
    ON_CALL(*this, getMetricValue).WillByDefault(::testing::Return(value_));
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  void delegateToFake(std::function<DataVariant()> callback) {
    ON_CALL(*this, size).WillByDefault(::testing::Return(size_of(callback())));
    ON_CALL(*this, getMetricValue).WillByDefault(callback);
    ON_CALL(*this, getDataType).WillByDefault(::testing::Return(type_));
  }

  void delegateToFake(std::function<DataVariant()> reader,
                      std::function<void(DataVariant)> writer) {
    ON_CALL(*this, size).WillByDefault(::testing::Return(size_of(reader())));
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