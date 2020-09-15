#ifndef __DATA_MODEL_METRIC_MOCK_HPP_
#define __DATA_MODEL_METRIC_MOCK_HPP_

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
  MockMetric(const std::string &REF_ID, const std::string &NAME,
             const std::string &DESC, DataType data_type)
      : Metric(REF_ID, NAME, DESC), data_type_(data_type) {}

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));

private:
  DataType data_type_;
};
} // namespace testing
} // namespace Information_Model

#endif //__DATA_MODEL_METRIC_MOCK_HPP_