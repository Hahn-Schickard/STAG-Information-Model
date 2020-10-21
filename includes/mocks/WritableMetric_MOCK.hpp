#ifndef __INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_
#define __INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_

#include "Metric_MOCK.hpp"
#include "WritableMetric.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
namespace testing {
/**
 * @brief WritableMetric mock implementation. Use only for testing!
 *
 */
class MockWritableMetric : public WritableMetric {
public:
  MockWritableMetric(const std::string &ref_id, const std::string &name,
                     const std::string &desc)
      : WritableMetric(ref_id, name, desc) {}

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(void, setMetricValue, (DataVariant), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));
};

using MockWritableMetricPtr = std::shared_ptr<MockWritableMetric>;
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITABLE_METRIC_FAKE_HPP_