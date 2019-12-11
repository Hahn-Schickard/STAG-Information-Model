#ifndef __READABLE_METRIC_FAKE_HPP_
#define __READABLE_METRIC_FAKE_HPP_

#include "ReadableMetric.hpp"

namespace Information_Model {
  namespace testing {
    /**
   * @brief ReadableMetric fake implementation. Use only for testing!
   * 
   * @tparam T - type of metric data type
   */
    template<class T>
    class FakeReadableMetric : public ReadableMetric<T> {
     public:
      FakeReadableMetric(const std::string& REF_ID,
          const std::string& NAME,
          const std::string& DESC,
          T value)
          : ReadableMetric<T>(REF_ID, NAME, DESC)
          , value_(DataWrapper<T>(value)) {}

      DataWrapper<T> getMetricValue() {
	return value_;
      }

     private:
      DataWrapper<T> value_;
    };
  }   // namespace testing
}   // namespace Information_Model
#endif   //__READABLE_METRIC_FAKE_HPP_