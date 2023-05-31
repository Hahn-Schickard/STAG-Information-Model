#ifndef __INFORMATION_MODEL_WRITEABLE_METRIC_HPP
#define __INFORMATION_MODEL_WRITEABLE_METRIC_HPP

#include "Metric.hpp"

namespace Information_Model {
/**
 * @brief A read and writable metric.
 *
 */
struct WritableMetric : public Metric {
  /**
   * @brief Writes the given DataVariant as a metric value to the modeled
   * sensor/actor system
   *
   * @throws std::invalid_argument if provided argument does not match the
   * modeled value type
   * @throws std::logic_error if internal setter callback does not exist
   *
   */
  virtual void setMetricValue(DataVariant /*value*/) {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::setMetricValue()");
  }

  /**
   * @brief Checks if the modeled metric does not supports value reading
   *
   * @return bool
   */
  virtual bool isWriteOnly() {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::isWriteOnly()");
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
