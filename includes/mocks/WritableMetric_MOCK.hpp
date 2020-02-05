#ifndef __DATA_MODEL_WRITABLE_METRIC_FAKE_HPP_
#define __DATA_MODEL_WRITABLE_METRIC_FAKE_HPP_

#include "../WritableMetric.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
namespace testing {
/**
 * @brief WritableMetric mock implementation. Use only for testing!
 *
 */
class MockWritableMetric : public WritableMetric {
public:
  MockWritableMetric(const std::string &REF_ID, const std::string &NAME,
                     const std::string &DESC, DataType data_type)
      : WritableMetric(REF_ID, NAME, DESC), data_type_(data_type) {}

  MOCK_METHOD(DataVariant, getMetricValue, (), (override));
  MOCK_METHOD(void, setMetricValue, (DataVariant), (override));
  MOCK_METHOD(DataType, getDataType, (), (override));

private:
  DataType data_type_;
};
} // namespace testing
} // namespace Information_Model
#endif //__DATA_MODEL_WRITABLE_METRIC_FAKE_HPP_