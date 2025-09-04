#ifndef __STAG_INFORMATION_MODEL_READABLE_HPP
#define __STAG_INFORMATION_MODEL_READABLE_HPP

#include "DataVariant.hpp"

#include <memory>
#include <stdexcept>
#include <string>

namespace Information_Model {
/**
 * @addtogroup ReadableModeling Metric Modelling
 * @{
 */

struct ReadCallbackUnavailable : public std::runtime_error {
  ReadCallbackUnavailable()
      : std::runtime_error("Read Callback is no longer available") {}
};

/**
 * @brief An interface to read only Metric.
 *
 * Models a single read only element for various sensors/actors
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addReadableMetric()
 */
struct Readable {
  virtual ~Readable() = default;

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
};

using ReadablePtr = std::shared_ptr<Readable>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_READABLE_HPP