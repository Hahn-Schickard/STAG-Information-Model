#ifndef __WRITABLE_METRIC_FAKE_HPP_
#define __WRITABLE_METRIC_FAKE_HPP_

#include "WritableMetric.hpp"

namespace Information_Model {
  namespace testing {
    /**
   * @brief WritableMetric fake implementation. Use only for testing!
   * 
   * @tparam T - type of metric data type 
   */
    template<class T>
    class FakeWritableMetric : public WritableMetric<T> {
     public:
      FakeWritableMetric(const std::string& REF_ID,
          const std::string& NAME,
          const std::string& DESC,
          T value)
          : WritableMetric<T>(REF_ID, NAME, DESC)
          , value_(DataWrapper<T>(value)) {}

      void setMetricValue(DataWrapper<T> value) {
	value_ = value;
      }

      DataWrapper<T> getMetricValue() {
	return value_;
      }

     private:
      DataWrapper<T> value_;
    };
  }   // namespace testing
}   // namespace Information_Model
#endif   //__WRITABLE_METRIC_FAKE_HPP_