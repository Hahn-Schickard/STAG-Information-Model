#ifndef __INFORMATION_MODEL_WRITEABLE_METRIC_HPP
#define __INFORMATION_MODEL_WRITEABLE_METRIC_HPP

#include "Metric.hpp"

namespace Information_Model {
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
 * built via DeviceBuilderInterface::addWritableMetric() or
 * DeviceBuilderInterface::addDeviceElement() with type set to
 * ElementType::WRITABLE
 */
struct WritableMetric : public Metric {
  virtual DataVariant getMetricValue() {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::getMetricValue()");
  }

  /**
   * @brief Writes the given DataVariant as a metric value to the modeled
   * sensor/actor system
   *
   * @throws std::invalid_argument if provided argument does not match the
   * modeled value type
   *
   */
  virtual void setMetricValue(DataVariant /*value*/) {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::setMetricValue()");
  }

  virtual DataType getDataType() {
    throw std::runtime_error(
        "Called based implementation of WritableMetric::getDataType()");
  }

  /**
   * @brief Checks if the modeled metric does not supports value reading
   *
   * @return true
   * @return false
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
