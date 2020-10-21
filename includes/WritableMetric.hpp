#ifndef __INFORMATION_MODEL_WRITEABLE_METRIC_HPP
#define __INFORMATION_MODEL_WRITEABLE_METRIC_HPP

#include "DataVariant.hpp"
#include "DeviceElement.hpp"

#include <string>

namespace Information_Model {
class WritableMetric : public DeviceElement {
protected:
  WritableMetric(const std::string &ref_id, const std::string &name,
                 const std::string &desc)
      : DeviceElement(ref_id, name, desc, ElementType::WRITABLE) {}

public:
  virtual DataVariant getMetricValue() = 0;
  virtual void setMetricValue(DataVariant value) = 0;
  virtual DataType getDataType() = 0;
};

using WritableMetricPtr = std::shared_ptr<WritableMetric>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITEABLE_METRIC_HPP
