#ifndef __STAG_INFORMATION_MODEL_WRITEABLE_HPP
#define __STAG_INFORMATION_MODEL_WRITEABLE_HPP

#include "Readable.hpp"

namespace Information_Model {
/**
 * @addtogroup WritableModeling Writable Metric Modelling
 * @{
 */
struct NonReadable : public std::runtime_error {
  NonReadable() : std::runtime_error("Metric is write only") {}
};

struct WriteCallbackUnavailable : public std::runtime_error {
  WriteCallbackUnavailable()
      : std::runtime_error("Write Callback is no longer available") {}
};
/**
 * @brief An interface to a read and writable Metric.
 *
 * Models a single writable element for various sensors/actors
 *
 * If this metric does not support active value reading (the readable part), all
 * calls to getMetricValue() will return the default value for set DataVariant
 * type
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addWritableMetric()
 */
struct Writable : virtual public Readable {
  virtual ~Writable() = default;

  /**
   * @brief Checks if the modeled metric does not supports value reading
   *
   * @return bool
   */
  virtual bool isWriteOnly() const = 0;

  /**
   * @brief Writes the given DataVariant as a metric value to the modeled
   * sensor/actor system
   *
   * @throws std::invalid_argument - if provided argument does not match the
   * modeled value type
   * @throws std::logic_error - if internal callback does not exist
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   */
  virtual void write(const DataVariant& /*value*/) const = 0;
};

using WritablePtr = std::shared_ptr<Writable>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_WRITEABLE_HPP
