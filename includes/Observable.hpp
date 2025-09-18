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
 * @brief Manages the life-time of the Observable subscription
 *
 * As long as this object and the source Observable are alive, new notification
 * calls will be dispatched to the attached ObserveCallback.
 *
 */
struct Observer {
  virtual ~Observer() = default;
};

using ObserverPtr = std::shared_ptr<Observer>;

/**
 * @brief An interface to a an observable metric with read support.
 *
 * Models a single observable element for various sensors/actors that informs
 * observers with new values when they are available
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilder::addObservable()
 */
struct Observable {
  using ObserveCallback =
      std::function<void(const std::shared_ptr<DataVariant>&)>;
  using ExceptionHandler = std::function<void(const std::exception_ptr&)>;

  virtual ~Observable() = default;

  /**
   * @brief Returns the modeled data type
   *
   * @return DataType
   */
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
   * @brief Attach a new observer to this source
   *
   * @param observe_cb - called when new notification is available
   * @param handler - called when observe_cb throws an exception
   * @return ObserverPtr
   */
  [[nodiscard]] virtual ObserverPtr subscribe(
      const ObserveCallback& observe_cb, const ExceptionHandler& handler) = 0;
};

using ObservablePtr = std::shared_ptr<Observable>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_OBSERVABLE_HPP