#ifndef __READABLE_METRIC_FAKE_HPP_
#define __READABLE_METRIC_FAKE_HPP_

#include "ReadableMetric.hpp"

using namespace Information_Model;

template<class T>
class FakeReadableMetric : public ReadableMetric<T> {
 public:
  FakeReadableMetric(const std::string REF_ID,
      const std::string NAME,
      const std::string DESC,
      T value)
      : ReadableMetric<T>(REF_ID, NAME, DESC)
      , value_(DataWrapper<T>(value)) {}

  DataWrapper<T> getMetricValue() {
    return value_;
  }

 private:
  DataWrapper<T> value_;
};

#endif   //__READABLE_METRIC_FAKE_HPP_