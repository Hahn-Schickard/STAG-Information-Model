#ifndef __INFORMATION_MODEL_WRITEABLE_METRIC_HPP
#define __INFORMATION_MODEL_WRITEABLE_METRIC_HPP

#include "Metric.hpp"

namespace Information_Model {
/**
 * @brief A read and writable metric. If this metric does not support active
 * value reading (Readable part), Information_Model::DataVariant will be set to
 * default data type value (for example: bool = true, int = 0, string = "");
 *
 */
struct WritableMetric : public Metric {
  virtual void setMetricValue(DataVariant /*value*/) {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::setMetricValue()");
  }

  virtual ~WritableMetric() = default;

protected:
  WritableMetric() = default;
};

using WritableMetricPtr = std::shared_ptr<WritableMetric>;
using NonemptyWritableMetricPtr =
    NonemptyPointer::NonemptyPtr<WritableMetricPtr>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITEABLE_METRIC_HPP
