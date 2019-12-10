#ifndef __WRITEABLE_METRIC_HPP
#define __WRITEABLE_METRIC_HPP

#include "ReadableMetric.hpp"

namespace Information_Model {
  /**
  * @brief A writeable metric type
  * 
  * This metric type also allows read access.
  * 
  * @attention This class should be built by Model_Factory::DeviceBuilder only!
  * 
  * @tparam T 
  */
  template<class T>
  class WritableMetric : public ReadableMetric<T> {
   protected:
    WritableMetric(const std::string REF_ID,
        const std::string NAME,
        const std::string DESC)
        : ReadableMetric<T>(REF_ID, NAME, DESC) {
      Metric::setElementType(ElementType::Writable);
    }

   public:
    void setMetricValue(DataWrapper<T> value);
  };
}   // namespace Information_Model
#endif   //__WRITEABLE_METRIC_HPP