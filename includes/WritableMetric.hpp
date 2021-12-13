#ifndef __INFORMATION_MODEL_WRITEABLE_METRIC_HPP
#define __INFORMATION_MODEL_WRITEABLE_METRIC_HPP

#include "DataVariant.hpp"
#include "DeviceElement.hpp"

#include <stdexcept>
#include <string>

namespace Information_Model {
/**
 * @brief A read and writable metric. If this metric does not support active
 * value reading (Readable part), Information_Model::DataVariant will be set to
 * default data type value (for example: bool = true, int = 0, string = "");
 *
 */
class WritableMetric : public DeviceElement {
protected:
  WritableMetric(const std::string &ref_id, const std::string &name,
                 const std::string &desc)
      : DeviceElement(ref_id, name, desc, ElementType::WRITABLE) {}

public:
  virtual DataVariant getMetricValue() {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::getMetricValue()");
  }

  virtual void setMetricValue(DataVariant /*value*/) {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::setMetricValue()");
  }

  virtual DataType getDataType() {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::getDataType()");
  }

  virtual ~WritableMetric() = default;
};

using WritableMetricPtr = std::shared_ptr<WritableMetric>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITEABLE_METRIC_HPP
