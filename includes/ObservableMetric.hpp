#ifndef __INFORMATION_MODEL_OBSERVABLE_METRIC_HPP
#define __INFORMATION_MODEL_OBSERVABLE_METRIC_HPP

#include "Event_Model/AsyncEventSource.hpp"
#include "Metric.hpp"

#include <functional>

namespace Information_Model {

using ObservedValueChange = std::shared_ptr<DataVariant>;

struct ObservableMetric
    : public Metric,
      public Event_Model::AsyncEventSource<ObservedValueChange> {
  virtual ~ObservableMetric() = default;

protected:
  ObservableMetric(DataType type,
      Event_Model::EventSourceInterface::ExceptionHandler handler)
      : Metric(type), AsyncEventSource(handler) {}
};

using ObservableMetricPtr = std::shared_ptr<ObservableMetric>;
using NonemptyObservableMetricPtr =
    NonemptyPointer::NonemptyPtr<ObservableMetricPtr>;

struct MetricObserver
    : public Event_Model::EventListenerInterface<ObservedValueChange> {
  virtual ~MetricObserver() = default;

protected:
  MetricObserver(ObservableMetricPtr source) : EventListenerInterface(source) {}

  MetricObserver(NonemptyObservableMetricPtr source)
      : EventListenerInterface(source.base()) {}
};

using MetricObserverPtr = std::shared_ptr<MetricObserver>;
} // namespace Information_Model