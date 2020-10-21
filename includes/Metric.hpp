#ifndef __INFORMATION_MODEL_METRIC_HPP
#define __INFORMATION_MODEL_METRIC_HPP

#include "DataVariant.hpp"
#include "DeviceElement.hpp"

#include <string>

namespace Information_Model {
class Metric : public DeviceElement {
protected:
  Metric(const std::string &ref_id, const std::string &name,
         const std::string &desc)
      : DeviceElement(ref_id, name, desc, ElementType::READABLE) {}

public:
  virtual DataVariant getMetricValue() = 0;
  virtual DataType getDataType() = 0;
  virtual ~Metric() = default;
};

using MetricPtr = std::shared_ptr<Metric>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_HPP