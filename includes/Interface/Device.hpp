#ifndef __STAG_INFORMATION_MODEL_DEVICE_HPP
#define __STAG_INFORMATION_MODEL_DEVICE_HPP

#include "Element.hpp"
#include "Group.hpp"
#include "MetaInfo.hpp"

#include <memory>
#include <stdexcept>
#include <string>

namespace Information_Model {
/**
 * @addtogroup DeviceModeling Device Modelling
 * @{
 */
/**
 * @brief An interface to the Device model
 *
 * Contains all of the modeled functionality of a singular addressable
 * sensor/actor system
 *
 * Each Device MUST have a reference ID (saved as ref_id), which will be used to
 * address any modeled functionality by Data Consumer Adapter Implementations
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface
 */
struct Device : virtual public MetaInfo {
  virtual ~Device() = default;

  virtual GroupPtr group() const = 0;

  virtual ElementPtr element(const std::string& ref_id) const = 0;
};

using DevicePtr = std::shared_ptr<Device>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_DEVICE_HPP