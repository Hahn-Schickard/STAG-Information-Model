#ifndef __INFORMATION_MODEL_METRIC_HPP
#define __INFORMATION_MODEL_METRIC_HPP

#include "DataVariant.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {
/**
 * @addtogroup ReadableModeling Metric Modelling
 * @{
 */
/**
 * @brief An interface to read only Metric.
 *
 * Models a single read only element for various sensors/actors
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addReadableMetric()
 */
struct Metric {
  /**
   * @brief Read the latest available metric value
   *
   * @throws std::logic_error if internal callback does not exist
   * @throws std::runtime_error if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   * @return DataVariant
   */
  virtual DataVariant getMetricValue() {
    throw std::logic_error(
        "Called based implementation of Metric::getMetricValue()");
  }

  virtual ~Metric() = default;

  /**
   * @brief Get the modeled data type
   *
   * @return DataType
   */
  DataType getDataType() const { return value_type_; }

  bool operator==(const Metric& other) const noexcept {
    return value_type_ == other.getDataType();
  }

  bool operator!=(const Metric& other) const noexcept {
    return !operator==(other);
  }

protected:
  Metric(DataType type) : value_type_(type) {}

  DataType value_type_ = DataType::UNKNOWN;
};

using MetricPtr = std::shared_ptr<Metric>;
using NonemptyMetricPtr = NonemptyPointer::NonemptyPtr<MetricPtr>;
/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_HPP