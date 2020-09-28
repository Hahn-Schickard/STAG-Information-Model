#ifndef __DATA_MODEL_METRIC_HPP
#define __DATA_MODEL_METRIC_HPP

#include "DataVariant.hpp"
#include "DeviceElement.hpp"

#include <string>

namespace Information_Model {
class Metric : public DeviceElement {
protected:
  Metric(const std::string REF_ID, const std::string NAME,
         const std::string DESC)
      : DeviceElement(REF_ID, NAME, DESC, ElementType::READABLE) {}

public:
  virtual DataVariant getMetricValue() = 0;
  virtual DataType getDataType() = 0;
  virtual ~Metric() = default;
};
} // namespace Information_Model
#endif //__DATA_MODEL_METRIC_HPP