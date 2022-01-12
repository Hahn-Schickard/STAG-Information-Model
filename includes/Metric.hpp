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
class Metric {
protected:
  Metric() = default;

public:
  virtual DataVariant getMetricValue() {
    throw std::runtime_error(
        "Called based implementation of Metric::getMetricValue()");
  }

  virtual DataType getDataType() {
    throw std::runtime_error(
        "Called based implementation of Metric::getDataType()");
  }

  virtual ~Metric() = default;
};

using MetricPtr = std::shared_ptr<Metric>;
using NonemptyMetricPtr = NonemptyPointer::NonemptyPtr<MetricPtr>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_HPP