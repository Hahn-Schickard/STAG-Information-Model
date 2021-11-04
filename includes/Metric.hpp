#ifndef __INFORMATION_MODEL_METRIC_HPP
#define __INFORMATION_MODEL_METRIC_HPP

#include "DataVariant.hpp"
#include "DeviceElement.hpp"

#include <string>

namespace Information_Model {
/**
 * @brief Read only Metric of a given type.
 *
 */
class Metric : public DeviceElement {
protected:
  Metric(const std::string &ref_id, const std::string &name,
         const std::string &desc)
      : DeviceElement(ref_id, name, desc, ElementType::READABLE) {}

public:
  virtual std::size_t size() {
    throw std::runtime_error("Called based implementation of Metric::size()");
  }

  virtual DataVariant getMetricValue() = 0;
  virtual DataType getDataType() = 0;

  virtual ~Metric() = default;
};

using MetricPtr = std::shared_ptr<Metric>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_METRIC_HPP