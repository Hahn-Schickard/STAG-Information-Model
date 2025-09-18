#ifndef __STAG_INFORMATION_MODEL_WRITEABLE_HPP
#define __STAG_INFORMATION_MODEL_WRITEABLE_HPP

#include <memory>
#include <stdexcept>
#include <string>

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
 * @brief An interface to a read and/or writable Metric.
 *
 * Models a single read and/or write element for various sensors/actors
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilder::addWritableMetric()
 */
struct Writable {
  virtual ~Writable() = default;

  /**
   * @brief Returns the modeled data type
   *
   * @return DataType
   */
  virtual DataType dataType() const = 0;

  /**
   * @brief Read the latest available metric value
   *
   * @throws NonReadable - if the modeled metric is write-only
   * @throws ReadCallbackUnavailable - if internal callback does not exist
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   * @return DataVariant
   */
  virtual DataVariant read() const = 0;

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
   * @throws WriteCallbackUnavailable - if internal callback does not exist
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   */
  virtual void write(const DataVariant&) const = 0;
};

using WritablePtr = std::shared_ptr<Writable>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_WRITEABLE_HPP
