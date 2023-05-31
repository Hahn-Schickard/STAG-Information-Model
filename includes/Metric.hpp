#ifndef __INFORMATION_MODEL_METRIC_HPP
#define __INFORMATION_MODEL_METRIC_HPP

#include "DataVariant.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {
/**
 * @brief Read only Metric of a given type.
 *
 */
struct Metric {
  /**
   * @brief Read the latest available metric value
   *
   * @throws std::logic_error if internal getter callback does not exist
   *
   * @return DataVariant
   */
  virtual DataVariant getMetricValue() {
    throw std::runtime_error(
        "Called based implementation of Metric::getMetricValue()");
  }

  /**
   * @brief Get the modeled data type
   *
   * @return DataType
   */
  virtual DataType getDataType() {
    throw std::runtime_error(
        "Called based implementation of Metric::getDataType()");
  }

  virtual ~Metric() = default;

protected:
  Metric() = default;
};

using MetricPtr = std::shared_ptr<Metric>;
using NonemptyMetricPtr = NonemptyPointer::NonemptyPtr<MetricPtr>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_HPP