#ifndef __STAG_INFORMATION_MODEL_META_INFO_HPP_
#define __STAG_INFORMATION_MODEL_META_INFO_HPP_

#include <memory>
#include <string>

namespace Information_Model {

/**
 * @brief Base modeling entity for Device and Element instances
 *
 */
struct MetaInfo {
  virtual ~MetaInfo() = default;

  virtual std::string id() const = 0;

  virtual std::string name() const = 0;

  virtual std::string description() const = 0;
};

using MetaInfoPtr = std::shared_ptr<MetaInfo>;
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_META_INFO_HPP_