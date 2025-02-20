#ifndef __INFORMATION_MODEL_OBSERVABLE_METRIC_HPP
#define __INFORMATION_MODEL_OBSERVABLE_METRIC_HPP

#include "Event_Model/AsyncEventSource.hpp"
#include "Event_Model/EventListenerInterface.hpp"
#include "Metric.hpp"

#include <functional>

namespace Information_Model {
/**
 * @addtogroup ObservableModeling Observable Metric Modelling
 * @{
 */

/**
 * @brief An interface to a an observable Metric with read support.
 *
 * Models a single observable element for various sensors/actors that informs
 * observers with new values
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addWritableMetric()
 */
struct ObservableMetric : public Metric,
                          public Event_Model::AsyncEventSource<DataVariant> {
  /**
   * @brief Callback to handle exceptions thrown in AsyncEventSource during
   * notify() execution
   *
   */
  using ExceptionHandler = std::function<void(const std::exception_ptr&)>;

  virtual ~ObservableMetric() = default;

  /**
   * @brief Informs all registered observers that a new DataVariant value has
   * been observed
   *
   * @param value - new observation
   */
  void observed(const DataVariant& value) {
    notify(std::make_shared<DataVariant>(value));
  }

protected:
  ObservableMetric(DataType type, const ExceptionHandler& handler)
      : Metric(type), AsyncEventSource(handler) {}
};

using ObservableMetricPtr = std::shared_ptr<ObservableMetric>;
using NonemptyObservableMetricPtr = Nonempty::Pointer<ObservableMetricPtr>;

/**
 * @brief An interface for ObservableMetric value change observation
 *
 * Anyone whishing to observe ObservableMetric value changes SHOULD inherit this
 * class and implement the handleEvent(DataVariantPtr) method
 *
 */
struct MetricObserver
    : public Event_Model::EventListenerInterface<DataVariant> {
  virtual ~MetricObserver() = default;

protected:
  explicit MetricObserver(const ObservableMetricPtr& source)
      : EventListenerInterface(source) {}

  explicit MetricObserver(const NonemptyObservableMetricPtr& source)
      : EventListenerInterface(source.base()) {}
};

using MetricObserverPtr = std::shared_ptr<MetricObserver>;
/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_OBSERVABLE_METRIC_HPP