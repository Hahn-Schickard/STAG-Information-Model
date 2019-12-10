#ifndef __READABLE_METRIC_HPP
#define __READABLE_METRIC_HPP

#include "DataWrapper.hpp"
#include "Metric.hpp"

#include <string>

namespace Information_Model {

  /**
  * @brief A  metric type
  * 
  * @attention This class should be built by Model_Factory::DeviceBuilder only!
  * 
  * @tparam T 
  */
  template<class T>
  class ReadableMetric : public Metric {
   protected:
    ReadableMetric(const std::string REF_ID,
        const std::string NAME,
        const std::string DESC)
        : Metric(REF_ID, NAME, DESC, ElementType::Readable, getDataType()) {}

   public:
    DataWrapper<T> getMetricValue();

    DataType getDataType() {
      if(std::is_same<T, bool>::value) {
	return DataType::BOOLEAN;
      } else if(std::is_same<T, uint8_t>::value) {
	return DataType::BYTE;
      } else if(std::is_same<T, int16_t>::value) {
	return DataType::SHORT;
      } else if(std::is_same<T, int32_t>::value) {
	return DataType::INTEGER;
      } else if(std::is_same<T, int64_t>::value) {
	return DataType::LONG;
      } else if(std::is_same<T, float>::value) {
	return DataType::FLOAT;
      } else if(std::is_same<T, double>::value) {
	return DataType::DOUBLE;
      } else if(std::is_same<T, std::string>::value) {
	return DataType::STRING;
      } else {
	return DataType::UNKNOWN;
      }
    }
  };
}   // namespace Information_Model
#endif   //__READABLE_METRIC_HPP