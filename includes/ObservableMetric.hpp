#ifndef __INFORMATION_MODEL_OBSERVABLE_METRIC_HPP
#define __INFORMATION_MODEL_OBSERVABLE_METRIC_HPP

#include "Event_Model/AsyncEventSource.hpp"
#include "Event_Model/EventListenerInterface.hpp"
#include "Metric.hpp"

#include <functional>

namespace Information_Model {

struct ObservableMetric : public Metric,
                          public Event_Model::AsyncEventSource<DataVariant> {
  using ExceptionHandler = std::function<void(const std::exception_ptr&)>;

  virtual ~ObservableMetric() = default;

  void observed(const std::shared_ptr<DataVariant>& value) { notify(value); }

protected:
  ObservableMetric(DataType type, ExceptionHandler handler)
      : Metric(type), AsyncEventSource(handler) {}
};

using ObservableMetricPtr = std::shared_ptr<ObservableMetric>;
using NonemptyObservableMetricPtr =
    NonemptyPointer::NonemptyPtr<ObservableMetricPtr>;

struct MetricObserver
    : public Event_Model::EventListenerInterface<DataVariant> {
  virtual ~MetricObserver() = default;

protected:
  MetricObserver(ObservableMetricPtr source) : EventListenerInterface(source) {}

  MetricObserver(NonemptyObservableMetricPtr source)
      : EventListenerInterface(source.base()) {}
};

using MetricObserverPtr = std::shared_ptr<MetricObserver>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_OBSERVABLE_METRIC_HPP