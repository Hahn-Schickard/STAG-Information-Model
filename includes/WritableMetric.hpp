#ifndef __INFORMATION_MODEL_WRITEABLE_METRIC_HPP
#define __INFORMATION_MODEL_WRITEABLE_METRIC_HPP

#include "Metric.hpp"

namespace Information_Model {
/**
 * @addtogroup WritableModeling Writable Metric Modelling
 * @{
 */
/**
 * @brief An interface to a read and writable Metric.
 *
 * Models a single writable element for various sensors/actors
 *
 * If this metric does not support active value reading (the readable part), all
 * calls to getMetricValue() will return the default value for set DataVariant
 * type
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addWritableMetric()
 */
struct WritableMetric : public Metric {
  /**
   * @brief Writes the given DataVariant as a metric value to the modeled
   * sensor/actor system
   *
   * @throws std::invalid_argument - if provided argument does not match the
   * modeled value type
   * @throws std::logic_error - if internal callback does not exist
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   */
  virtual void setMetricValue(DataVariant /*value*/) {
    throw std::logic_error(
        "Called based implementation of WritableMetric::setMetricValue()");
  }

  /**
   * @brief Checks if the modeled metric does not supports value reading
   *
   * @return bool
   */
  virtual bool isWriteOnly() {
    throw std::logic_error(
        "Called based implementation of WritableMetric::isWriteOnly()");
  }

  virtual ~WritableMetric() = default;

protected:
  WritableMetric(DataType type) : Metric(type) {}
};

using WritableMetricPtr = std::shared_ptr<WritableMetric>;
using NonemptyWritableMetricPtr = Nonempty::Pointer<WritableMetricPtr>;
/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_WRITEABLE_METRIC_HPP
