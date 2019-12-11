#ifndef __METRIC_FAKE_HPP_
#define __METRIC_FAKE_HPP_

#include "Metric.hpp"

namespace Information_Model {
  namespace testing {

    /**
   * @brief Metric fake implementation. Use only for testing!
   * 
   */
    class FakeMetric : public Metric {
     public:
      FakeMetric(const std::string& REF_ID,
          const std::string& NAME,
          const std::string& DESC,
          ElementType type,
          DataType data_type)
          : Metric(REF_ID, NAME, DESC, type, data_type) {}
    };
  }   // namespace testing
}   // namespace Information_Model

#endif   //__METRIC_FAKE_HPP_