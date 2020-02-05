#ifndef __DATA_MODEL_WRITEABLE_METRIC_HPP
#define __DATA_MODEL_WRITEABLE_METRIC_HPP

#include "DataVariant.hpp"
#include "DeviceElement.hpp"

#include <string>

namespace Information_Model {
class WritableMetric : public DeviceElement {
protected:
  WritableMetric(const std::string &REF_ID, const std::string &NAME,
                 const std::string &DESC)
      : DeviceElement(REF_ID, NAME, DESC, ElementType::WRITABLE) {}

public:
  virtual DataVariant getMetricValue() = 0;
  virtual void setMetricValue(DataVariant value) = 0;
  virtual DataType getDataType() = 0;
};
} // namespace Information_Model
#endif //__DATA_MODEL_WRITEABLE_METRIC_HPP
