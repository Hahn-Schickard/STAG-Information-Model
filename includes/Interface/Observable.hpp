#ifndef __STAG_INFORMATION_MODEL_OBSERVABLE_HPP
#define __STAG_INFORMATION_MODEL_OBSERVABLE_HPP

#include "DataVariant.hpp"

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

namespace Information_Model {
/**
 * @addtogroup ObservableModeling Observable Metric Modelling
 * @{
 */

/**
 * @brief An interface for ObservableMetric value change observation
 *
 * Anyone whishing to observe ObservableMetric value changes SHOULD inherit this
 * class and implement the handleEvent(DataVariantPtr) method
 *
 */
struct Observer {
  virtual ~Observer() = default;
};

using ObserverPtr = std::shared_ptr<Observer>;

/**
 * @brief An interface to a an observable Metric with read support.
 *
 * Models a single observable element for various sensors/actors that informs
 * observers with new values
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addObservable()
 */
struct Observable {
  using ObserveCallback =
      std::function<void(const std::shared_ptr<DataVariant>&)>;
  using ExceptionHandler = std::function<void(const std::exception_ptr&)>;

  virtual ~Observable() = default;

  virtual DataType dataType() const = 0;

  /**
   * @brief Read the latest available metric value
   *
   * @throws ReadCallbackUnavailable - if internal callback does not exist
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   * @return DataVariant
   */
  virtual DataVariant read() const = 0;

  /**
   * @brief Informs all registered observers that a new DataVariant value has
   * been observed
   *
   * @param value - new observation
   */
  virtual void notify(const DataVariant& value) const = 0;

  [[nodiscard]] virtual ObserverPtr subscribe(
      const ObserveCallback& observe_cb, const ExceptionHandler& handler) = 0;
};

using ObservablePtr = std::shared_ptr<Observable>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_OBSERVABLE_HPP