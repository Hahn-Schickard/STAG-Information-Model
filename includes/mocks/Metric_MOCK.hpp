#ifndef __INFORMATION_MODEL_METRIC_MOCK_HPP_
#define __INFORMATION_MODEL_METRIC_MOCK_HPP_

#include "Metric.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
namespace testing {
/**
 * @brief Metric fake implementation. Use only for testing!
 *
 */
class MockMetric : public Metric {
public:
  MockMetric(const std::string &ref_id, const std::string &name,
             const std::string &desc)
      : Metric(ref_id, name, desc) {}

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));
};

using MockMetricPtr = std::shared_ptr<MockMetric>;
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_MOCK_HPP_